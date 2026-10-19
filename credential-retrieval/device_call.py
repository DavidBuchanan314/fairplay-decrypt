#!/usr/bin/env python3
"""Invoke the entitled selector-21 helper through Frida.

Frida is used only to transfer files and spawn a separately entitled helper.
The helper, not fairplayd, opens FairPlayIOKit and invokes selector 21. Input
paths on the device are unique and never overwritten; remote files are kept
for audit. A matching jailbreak/Frida transport and the signed helper are
required.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import sys
import threading
import time

import frida

ROOT = Path(__file__).resolve().parent
HELPER = ROOT / "helper.ios14"
PREFIX = 0x160
REQUEST_SIZE = 552
ARTIFACT_SIZE = PREFIX + REQUEST_SIZE + 16


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run_agent(
    device, source: str, expected_messages: int
) -> list[tuple[dict, bytes | None]]:
    """Run a short file I/O script in a disposable unsandboxed /bin/sh child."""
    pid = device.spawn(["/bin/sh"])
    session = None
    script = None
    records: list[tuple[dict, bytes | None]] = []
    received = threading.Event()
    try:
        session = device.attach(pid)

        def on_message(message, data):
            payload = message.get("payload", message)
            records.append((payload, data))
            if len(records) >= expected_messages or message.get("type") == "error":
                received.set()

        script = session.create_script(source)
        script.on("message", on_message)
        script.load()
        if not received.wait(10):
            raise TimeoutError(
                f"temporary Frida agent returned {len(records)} messages"
            )
        for payload, _ in records:
            if not isinstance(payload, dict) or payload.get("error"):
                raise RuntimeError(f"temporary Frida agent error: {payload}")
        return records
    finally:
        if script is not None:
            script.unload()
        if session is not None:
            session.detach()
        device.kill(pid)


def upload(device, files: list[tuple[str, bytes]]) -> None:
    records = [{"path": path, "hex": data.hex()} for path, data in files]
    source = """
const records = __RECORDS__;
const open = new NativeFunction(Module.getGlobalExportByName('open'), 'int',
                                ['pointer', 'int', 'int']);
const write = new NativeFunction(Module.getGlobalExportByName('write'), 'ssize_t',
                                 ['int', 'pointer', 'size_t']);
const fchmod = new NativeFunction(Module.getGlobalExportByName('fchmod'), 'int',
                                  ['int', 'int']);
const fsync = new NativeFunction(Module.getGlobalExportByName('fsync'), 'int', ['int']);
const close = new NativeFunction(Module.getGlobalExportByName('close'), 'int', ['int']);
for (const item of records) {
  const fd = open(Memory.allocUtf8String(item.path), 0xA01, 0o600);
  if (fd < 0) { send({error: 'exclusive create failed', path: item.path}); continue; }
  const bytes = item.hex.match(/../g).map(value => parseInt(value, 16));
  const buffer = Memory.alloc(bytes.length);
  buffer.writeByteArray(bytes);
  let at = 0;
  while (at < bytes.length) {
    const count = Number(write(fd, buffer.add(at), bytes.length - at));
    if (count <= 0) break;
    at += count;
  }
  const mode = item.path.endsWith('.ios14') ? 0o755 : 0o600;
  const chmodStatus = fchmod(fd, mode);
  const syncStatus = fsync(fd);
  const closeStatus = close(fd);
  if (at !== bytes.length || chmodStatus !== 0 || syncStatus !== 0 || closeStatus !== 0) {
    send({error: 'short or failed remote write', path: item.path, at,
          length: bytes.length, chmodStatus, syncStatus, closeStatus});
    continue;
  }
  const reread = File.readAllBytes(item.path);
  send({kind: 'uploaded', path: item.path, length: reread.byteLength}, reread);
}
""".replace("__RECORDS__", json.dumps(records))
    results = run_agent(device, source, len(files))
    by_path = {payload.get("path"): data for payload, data in results}
    for path, expected in files:
        observed = by_path.get(path)
        if observed is None or observed != expected:
            raise AssertionError(f"remote upload/readback mismatch: {path}")
        print(f"uploaded {path} length={len(expected)} sha256={sha256(expected)}")


def download(device, path: str) -> bytes:
    source = (
        "try { const data = File.readAllBytes("
        + json.dumps(path)
        + "); send({kind:'artifact',length:data.byteLength},data); } "
        "catch (error) { send({error:String(error)}); }"
    )
    records = run_agent(device, source, 1)
    payload, data = records[0]
    if (
        payload.get("kind") != "artifact"
        or data is None
        or len(data) != payload["length"]
    ):
        raise AssertionError("remote artifact missing or truncated")
    return data


def invoke_helper(device, argv: list[str]) -> str:
    output: list[str] = []

    def on_output(pid, fd, data):
        if data:
            output.append(data.decode("utf-8", "replace"))

    device.on("output", on_output)
    pid = device.spawn(argv, stdio="pipe")
    device.resume(pid)
    deadline = time.monotonic() + 16
    while time.monotonic() < deadline:
        time.sleep(0.25)
        if pid not in {process.pid for process in device.enumerate_processes()}:
            time.sleep(0.1)
            return "".join(output)
    device.kill(pid)
    raise TimeoutError(
        "helper exceeded its eight-second alarm and 16-second host bound"
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--helper", type=Path, default=HELPER)
    parser.add_argument("--request", type=Path, required=True)
    parser.add_argument("--source16", type=Path, required=True)
    parser.add_argument(
        "--slot-address", type=lambda value: int(value, 0), required=True
    )
    parser.add_argument(
        "--output",
        type=Path,
        required=True,
        help="new local 920-byte artifact; refuses overwrite",
    )
    parser.add_argument("--expected-udid", required=True)
    args = parser.parse_args()
    if args.output.exists():
        parser.error("local output already exists")
    helper = args.helper.read_bytes()
    request = args.request.read_bytes()
    source16 = args.source16.read_bytes()
    if len(request) != REQUEST_SIZE or len(source16) != 16:
        parser.error("request must be 552 bytes and side-slot source 16 bytes")
    if (
        args.slot_address < 0x100000000
        or args.slot_address >= 1 << 40
        or args.slot_address & 15
    ):
        parser.error("slot address must be 16-byte aligned and fit the encoder pointer")
    udid = args.expected_udid.strip().lower()
    device = frida.get_usb_device(timeout=10)
    if device.id.lower() != udid.lower():
        raise ValueError(f"connected device {device.id} differs from selected UDID")
    token = secrets.token_hex(5)
    remote = f"/var/root/fp-synthetic-{token}"
    helper_path = remote + ".ios14"
    request_path = remote + "-request.bin"
    source_path = remote + "-source16.bin"
    result_path = remote + "-result.bin"
    upload(
        device,
        [(helper_path, helper), (request_path, request), (source_path, source16)],
    )
    command = [
        helper_path,
        request_path,
        result_path,
        "--call",
        "--map-slot",
        source_path,
        hex(args.slot_address),
    ]
    log = invoke_helper(device, command)
    # ASLR may occupy the requested page before selector 21 is called.
    # Permit one new-process retry only for this allocation collision.
    slot = re.search(r"slot-map-status=(0x[0-9a-f]+)", log)
    if slot is not None and int(slot.group(1), 16) != 0 and "call-status=" not in log:
        print(log.rstrip())
        print("requested slot occupied in first helper process; retrying once")
        log = invoke_helper(device, command)
        slot = re.search(r"slot-map-status=(0x[0-9a-f]+)", log)
        if (
            slot is not None
            and int(slot.group(1), 16) != 0
            and "call-status=" not in log
        ):
            print(log.rstrip())
            sys.exit(8)
    print(log.rstrip())
    for expected in (
        "open-status=0",
        "slot-map-status=0",
        "call-status=0",
        "output-size=552",
        "source16=" + source16.hex(),
    ):
        if expected not in log:
            raise AssertionError(f"standalone hardware log lacks {expected!r}")
    artifact = download(device, result_path)
    if len(artifact) != ARTIFACT_SIZE or artifact[:PREFIX] != bytes([0x5A]) * PREFIX:
        raise AssertionError("hardware artifact length or untouched prefix differs")
    sidewrite = artifact[-16:]
    match = re.search(r"known-side-slot-first16=([0-9a-f]{32})", log)
    if match is None or bytes.fromhex(match.group(1)) != sidewrite:
        raise AssertionError("artifact sidewrite differs from helper's observed slot")
    descriptor = os.open(args.output, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(descriptor, "wb") as stream:
        stream.write(artifact)
    print(
        f"artifact={args.output} sha256={sha256(artifact)} "
        f"sidewrite={sidewrite.hex()}"
    )
    print(f"remote audit files retained under {remote}*")


if __name__ == "__main__":
    main()
