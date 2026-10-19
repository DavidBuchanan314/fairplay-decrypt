#!/usr/bin/env python3
"""Read the already-generated 0x899 accelerator key with bounded kernel reads.

This inspects the bounded __DATA segment of the loaded FairPlayIOKit image
for its IOAESAccelerator object pointer, then reads its special-key table.
It never writes kernel memory, starts an app decrypt, or inspects fairplayd
state. The key is a SIDB-unwrapping input, not the sought account credential.
"""

import argparse
import hashlib
import os
from pathlib import Path
import subprocess
import threading

import frida

SUPPORTED_BUILD = "18G82"


JS = r"""
setImmediate(function () {
  try {
    if (!Kernel.available) throw new Error('kernel read API unavailable');
    const module = Kernel.enumerateModules().find(m =>
      m.name === 'com.apple.driver.FairPlayIOKit');
    if (module === undefined) throw new Error('FairPlayIOKit not loaded');
    const u64 = value => new UInt64('0x' + value.toString(16));
    const plausible = value => value >= 0xffffffe000000000n &&
                               value < 0xfffffff000000000n &&
                               (value & 0x7n) === 0n;
    const header = new DataView(Kernel.readByteArray(module.base, 32));
    if (header.getUint32(0, true) !== 0xfeedfacf)
      throw new Error('loaded FairPlayIOKit is not Mach-O arm64');
    const commands = header.getUint32(16, true);
    const commandBytes = header.getUint32(20, true);
    if (commands < 2 || commands > 64 || commandBytes > 0x4000)
      throw new Error('unexpected FairPlayIOKit load commands');
    const image = new DataView(Kernel.readByteArray(module.base, 32 + commandBytes));
    let textVm = null, dataVm = null, dataSize = null, cursor = 32;
    for (let i = 0; i < commands; i++) {
      const cmd = image.getUint32(cursor, true);
      const size = image.getUint32(cursor + 4, true);
      if (size < 8 || cursor + size > image.byteLength)
        throw new Error('invalid FairPlayIOKit load command');
      if (cmd === 0x19 && size >= 72) {
        let name = '';
        for (let at = cursor + 8; at < cursor + 24; at++) {
          const code = image.getUint8(at);
          if (code === 0) break;
          name += String.fromCharCode(code);
        }
        if (name === '__TEXT') textVm = image.getBigUint64(cursor + 24, true);
        if (name === '__DATA') {
          dataVm = image.getBigUint64(cursor + 24, true);
          dataSize = Number(image.getBigUint64(cursor + 32, true));
        }
      }
      cursor += size;
    }
    if (textVm === null || dataVm === null || dataSize < 0x100 ||
        dataSize > 0x10000)
      throw new Error('unsupported FairPlayIOKit segment layout');
    const dataBase = dataVm + BigInt(module.base.toString()) - textVm;
    const globals = new DataView(Kernel.readByteArray(u64(dataBase), dataSize));
    const stride = 52;
    const matches = [], seen = new Set();
    let pointers = 0;
    for (let offset = 0; offset + 8 <= dataSize; offset += 8) {
      const object = globals.getBigUint64(offset, true);
      if (!plausible(object) || seen.has(object.toString())) continue;
      seen.add(object.toString());
      pointers++;
      try {
        const h = new DataView(Kernel.readByteArray(u64(object), 0xe0));
        const table = h.getBigUint64(0xd0, true);
        const count = h.getUint32(0xd8, true);
        if (!plausible(table) || count < 4 || count > 32) continue;
        const raw = new Uint8Array(Kernel.readByteArray(u64(table), count * stride));
        const view = new DataView(raw.buffer);
        const records = [];
        for (let index = 0; index < count; index++) {
          const at = index * stride;
          const generated = view.getUint32(at, true);
          const id = view.getUint32(at + 4, true);
          const bits = view.getUint32(at + 8, true);
          const key = raw.slice(at + 12, at + 28);
          records.push({index, generated, id, bits, nonzero:key.some(x => x !== 0)});
          if (id === 0x899 && generated === 1 && bits === 128 &&
              key.some(x => x !== 0)) {
            matches.push({slot:dataBase + BigInt(offset), object, table,
                          count, records, index, key});
          }
        }
      } catch (_) {
        // A data-segment pointer may be unrelated or temporarily unreadable.
      }
    }
    if (matches.length !== 1)
      throw new Error('expected one generated 0x899 key; found ' + matches.length);
    const match = matches[0];
    send({kind:'key', index:match.index, generated:1, id:0x899, bits:128},
         match.key.buffer);
    send({kind:'done', slot:'0x'+match.slot.toString(16),
      object:'0x'+match.object.toString(16), table:'0x'+match.table.toString(16),
      count:match.count, matchingIds:matches.length, scannedPointers:pointers,
      records:match.records});
  } catch (error) {
    send({kind:'error', message:String(error)});
  }
});
"""


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--device-udid", required=True)
    args = parser.parse_args()
    if args.output.exists():
        parser.error(f"refusing to replace existing file: {args.output}")
    udid = subprocess.run(
        ["idevice_id", "-l"], capture_output=True, text=True, timeout=5, check=True
    ).stdout.splitlines()
    if udid != [args.device_udid]:
        raise RuntimeError(
            f"expected only selected device {args.device_udid}, found {udid}"
        )
    build = subprocess.run(
        ["ideviceinfo", "-u", args.device_udid, "-k", "BuildVersion"],
        capture_output=True,
        text=True,
        timeout=5,
        check=True,
    ).stdout.strip()
    if build != SUPPORTED_BUILD:
        raise RuntimeError(f"unsupported build {build!r}; need {SUPPORTED_BUILD}")

    device = frida.get_usb_device(timeout=10)
    if device.id.lower() != args.device_udid.lower():
        raise RuntimeError("Frida selected a different USB device")
    session = device.attach(0)
    done = threading.Event()
    status = {}
    keys = []

    def on_message(message, data):
        if message.get("type") == "error":
            status["error"] = message.get("stack", str(message))
            done.set()
            return
        payload = message.get("payload", {})
        if payload.get("kind") == "key":
            keys.append((payload, data))
        elif payload.get("kind") == "done":
            status.update(payload)
            done.set()
        elif payload.get("kind") == "error":
            status["error"] = payload.get("message", "unknown error")
            done.set()

    script = session.create_script(JS)
    script.on("message", on_message)
    try:
        script.load()
        if not done.wait(15):
            raise TimeoutError("bounded accelerator-object read did not complete")
    finally:
        session.detach()
    if "error" in status:
        raise RuntimeError(status["error"])
    print(f"key-table entries={status['count']} 0x899 IDs={status['matchingIds']}")
    for record in status["records"]:
        print(
            f"entry {record['index']}: id={record['id']:#x} "
            f"generated={record['generated']} bits={record['bits']} "
            f"nonzero={record['nonzero']}"
        )
    if len(keys) != 1 or not isinstance(keys[0][1], bytes) or len(keys[0][1]) != 16:
        raise RuntimeError(f"expected one generated 16-byte 0x899 key, got {len(keys)}")
    key = keys[0][1]
    fd = os.open(args.output, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(fd, "wb") as stream:
        stream.write(key)
    print(
        f"saved accelerator key ID 0x899 to {args.output}; "
        f"sha256={hashlib.sha256(key).hexdigest()} (bytes not printed)"
    )


if __name__ == "__main__":
    main()
