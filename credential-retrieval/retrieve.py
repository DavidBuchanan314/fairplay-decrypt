#!/usr/bin/env python3
"""Retrieve an account credential on a supported 18G82 device."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import subprocess
import sys
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT.parent))

from core import derive_work16  # noqa: E402
from offline import (
    aes__core_decrypt,
    core3__derive,
    hardware__derive,
    priv_transform__atom,
    tail__final_pass,
    tail__first_pass,
    tail__table_from_static_init,
    tail__table_pass,
)  # noqa: E402

BUILD_ASSETS = {
    "fairplayd.A2.ios14": "30469ad8d24c232c277af6ce5a794b1fb36b4f25543b642be331d3b3f6b5d411",
    "static-key-table.bin": "c7b33507f71d5f7a07f623eb21f6b9f23997f950119371631f6535f63e6d2af1",
    "helper.ios14": "6e8872ac1dfefe66f761fd70f9f22aced398c1f42ae11bdd7eaaefcbf5c442b3",
}
CONTEXT_RE = re.compile(r"^keybag-output-first16=([0-9a-f]{32})$", re.MULTILINE)
SUPPORTED_BUILD = "18G82"
DEVICE_SIDB = "/private/var/mobile/Library/FairPlay/iTunes_Control/iTunes/IC-Info.sidb"
SLOT_IN_PAGE = 0x14F0
BODY_FROM_SLOT = 0x180
MAX_LICENSE_BYTES = 1 << 20


class CommandFailure(RuntimeError):
    def __init__(self, code: int, output: str):
        super().__init__(f"subprocess exited {code}")
        self.code = code
        self.output = output


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def write_new(path: Path, data: bytes) -> None:
    with os.fdopen(
        os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600), "wb"
    ) as stream:
        stream.write(data)


def run(command: list[str], env: dict[str, str], *, compact: bool = False) -> str:
    completed = subprocess.run(command, env=env, capture_output=True, text=True)
    output = completed.stdout + completed.stderr
    lines = output.splitlines()
    print("running:", " ".join(command), flush=True)
    print(
        "\n".join(
            lines[:1] + lines[-2:]
            if compact and completed.returncode == 0 and len(lines) > 3
            else lines
        ),
        flush=True,
    )
    if completed.returncode:
        raise CommandFailure(completed.returncode, output)
    return output


def preflight() -> None:
    for filename, expected in BUILD_ASSETS.items():
        if digest((ROOT / filename).read_bytes()) != expected:
            raise ValueError(f"missing or altered 18G82 asset: {filename}")


def connected_udid() -> str:
    devices = subprocess.run(
        ["idevice_id", "-l"], capture_output=True, text=True, check=True, timeout=10
    ).stdout.splitlines()
    if len(devices) != 1:
        raise RuntimeError(
            f"connect exactly one supported device; found {len(devices)}"
        )
    return normalize_udid(devices[0])


def normalize_udid(value: str) -> str:
    value = value.strip().lower()
    if not re.fullmatch(r"[0-9a-f]{40}", value):
        raise ValueError("device UDID must contain exactly 40 hexadecimal digits")
    return value


def check_build(udid: str) -> str:
    def info(key: str) -> str:
        return subprocess.run(
            ["ideviceinfo", "-u", udid, "-k", key],
            capture_output=True,
            text=True,
            check=True,
            timeout=10,
        ).stdout.strip()

    build = info("BuildVersion")
    if build != SUPPORTED_BUILD:
        raise ValueError(f"unsupported device build {build!r}; need {SUPPORTED_BUILD}")
    return info("ProductType")


def read_device_sidb(udid: str, path: str) -> bytes:
    import frida
    from device_call import download

    device = frida.get_usb_device(timeout=10)
    if device.id.lower() != udid:
        raise ValueError("Frida selected a different USB device")
    return download(device, path)


def license_sinf(
    raw_path: Path | None, ipa_path: Path | None, member: str | None
) -> tuple[bytes, str]:
    if raw_path is not None:
        if member:
            raise ValueError("--sinf-member requires --ipa")
        if raw_path.stat().st_size > MAX_LICENSE_BYTES:
            raise ValueError("license SINF is unreasonably large")
        sinf = raw_path.read_bytes()
        label = raw_path.name
    else:
        if ipa_path is None:
            raise ValueError("supply --sinf or --ipa")
        with ZipFile(ipa_path) as archive:
            members = sorted(
                name
                for name in archive.namelist()
                if "/SC_Info/" in name
                and name.endswith(".sinf")
                and not name.endswith(".v4.sinf")
            )
            if member:
                if member not in members:
                    raise ValueError("--sinf-member is not a supported SINF in the IPA")
                if archive.getinfo(member).file_size > MAX_LICENSE_BYTES:
                    raise ValueError("license SINF is unreasonably large")
                label = member
                sinf = archive.read(member)
            else:
                by_key: dict[tuple[bytes, bytes], list[tuple[str, bytes]]] = {}
                for name in members:
                    if archive.getinfo(name).file_size > MAX_LICENSE_BYTES:
                        continue
                    data = archive.read(name)
                    try:
                        key = (
                            priv_transform__atom(data, b"user"),
                            priv_transform__atom(data, b"key "),
                        )
                    except ValueError:
                        continue
                    by_key.setdefault(key, []).append((name, data))
                if len(by_key) != 1:
                    raise ValueError(
                        f"IPA has {len(by_key)} distinct license selectors; "
                        "choose one with --sinf-member"
                    )
                choices = next(iter(by_key.values()))
                label, sinf = min(
                    choices, key=lambda item: (item[0].count("/"), item[0])
                )
    if (
        len(sinf) < 8
        or sinf[4:8] != b"sinf"
        or int.from_bytes(sinf[:4], "big") != len(sinf)
    ):
        raise ValueError("input is not one complete SINF atom")
    return sinf, label


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--preflight",
        action="store_true",
        help="check bundled inputs without contacting the device",
    )
    parser.add_argument("--output-dir", type=Path, help="new directory; must not exist")
    parser.add_argument(
        "--sidb", type=Path, help="saved IC-Info.sidb; fetched from device when live"
    )
    parser.add_argument(
        "--device-sidb-path",
        default=DEVICE_SIDB,
        help="on-device SIDB path when --sidb is omitted",
    )
    license_group = parser.add_mutually_exclusive_group()
    license_group.add_argument("--sinf", type=Path, help="raw license SINF")
    license_group.add_argument(
        "--ipa", type=Path, help="authorized IPA containing a SINF"
    )
    parser.add_argument(
        "--sinf-member", help="IPA ZIP member if it has multiple license selectors"
    )
    parser.add_argument(
        "--device-udid", help="40-digit device UDID; discovered automatically when live"
    )
    parser.add_argument(
        "--uid-key-file", type=Path, help="offline verification: saved hardware UID key"
    )
    parser.add_argument(
        "--hardware-artifact",
        type=Path,
        help="offline verification: saved standalone selector result",
    )
    parser.add_argument(
        "--slot-address",
        type=lambda value: int(value, 0),
        help="16-byte side slot; required when replaying a saved hardware artifact",
    )
    args = parser.parse_args()
    preflight()
    if args.preflight:
        if any(
            (
                args.output_dir,
                args.uid_key_file,
                args.hardware_artifact,
                args.sidb,
                args.sinf,
                args.ipa,
                args.sinf_member,
                args.device_udid,
                args.slot_address,
            )
        ):
            parser.error("--preflight takes no retrieval inputs")
        print("18G82 assets verified; device not contacted")
        return
    if not args.output_dir or args.output_dir.exists():
        parser.error("--output-dir must name a new directory")
    if not args.sinf and not args.ipa:
        parser.error("--sinf or --ipa is required for retrieval")
    if not args.sidb and args.uid_key_file and args.hardware_artifact:
        parser.error("offline replay requires --sidb")
    if args.hardware_artifact and args.slot_address is None:
        parser.error("a saved hardware artifact requires its original --slot-address")
    if args.slot_address is not None:
        if not 0x100000000 <= args.slot_address < 1 << 40:
            parser.error("--slot-address must fit the 40-bit encoder pointer")
        if args.slot_address & 0x3FFF != SLOT_IN_PAGE:
            parser.error("--slot-address must end in a 16 KiB page offset of 0x14f0")
    sinf, sinf_label = license_sinf(args.sinf, args.ipa, args.sinf_member)
    offline = bool(args.uid_key_file and args.hardware_artifact)
    if offline and not args.device_udid:
        parser.error("offline replay requires --device-udid")
    udid = normalize_udid(args.device_udid) if args.device_udid else connected_udid()
    product_type = None if offline else check_build(udid)
    sidb = (
        args.sidb.read_bytes()
        if args.sidb
        else read_device_sidb(udid, args.device_sidb_path)
    )
    if len(sidb) < 36 or (len(sidb) - 20) % 16 or sidb[:4] != bytes.fromhex("80060003"):
        raise ValueError("unsupported SIDB version or length")
    os.umask(0o077)
    args.output_dir.mkdir(parents=True, mode=0o700)
    sidb_path = args.output_dir / "IC-Info.sidb"
    sinf_path = args.output_dir / "license.sinf"
    write_new(sidb_path, sidb)
    write_new(sinf_path, sinf)
    uid_path = args.output_dir / "uid899.bin"
    request_path = args.output_dir / "request.bin"
    source_path = args.output_dir / "source16.bin"
    artifact_path = args.output_dir / "hardware-result.bin"
    credential_path = args.output_dir / "credential.bin"
    env = os.environ.copy()
    env["FAIRPLAY_SIDB_FILE"] = str(sidb_path.resolve())
    env["FAIRPLAY_DEVICE_UDID"] = udid
    os.environ["FAIRPLAY_SIDB_FILE"] = env["FAIRPLAY_SIDB_FILE"]
    os.environ["FAIRPLAY_DEVICE_UDID"] = udid

    if args.uid_key_file:
        write_new(uid_path, args.uid_key_file.read_bytes())
        print("using saved UID-key checkpoint")
    else:
        run(
            [
                sys.executable,
                str(ROOT / "device_key.py"),
                "--device-udid",
                udid,
                "--output",
                str(uid_path),
            ],
            env,
        )
    uid = uid_path.read_bytes()
    if len(uid) != 16 or not any(uid):
        raise ValueError("missing or malformed UID key")
    env["FAIRPLAY_UID899_KEY_FILE"] = str(uid_path.resolve())
    os.environ["FAIRPLAY_UID899_KEY_FILE"] = env["FAIRPLAY_UID899_KEY_FILE"]

    fields = hardware__derive(sinf)
    source16 = fields["source16"]
    if len(source16) != 16:
        raise ValueError("selected hardware source is not 16 bytes")
    write_new(source_path, source16)

    slots = (
        [args.slot_address]
        if args.slot_address is not None
        else [
            0x16B000000 + page * 0x4000 + SLOT_IN_PAGE
            for page in secrets.SystemRandom().sample(range(0x4000), 3)
        ]
    )
    selected_slot = None
    for attempt, slot in enumerate(slots, 1):
        attempt_request = args.output_dir / f"request-attempt-{attempt}.bin"
        run(
            [
                sys.executable,
                str(ROOT / "request.py"),
                "--fully-synthetic-middle",
                "--license-sinf",
                str(sinf_path),
                "--synthetic-selector-body-address",
                hex(slot + BODY_FROM_SLOT),
                "--stop-offset",
                "0x199150",
                "--output-request",
                str(attempt_request),
            ],
            env,
            compact=True,
        )
        if len(attempt_request.read_bytes()) != 552:
            raise ValueError("synthetic selector request has unexpected length")
        if args.hardware_artifact:
            write_new(artifact_path, args.hardware_artifact.read_bytes())
            print("using saved hardware-result checkpoint")
        else:
            try:
                run(
                    [
                        sys.executable,
                        str(ROOT / "device_call.py"),
                        "--helper",
                        str(ROOT / "helper.ios14"),
                        "--request",
                        str(attempt_request),
                        "--source16",
                        str(source_path),
                        "--slot-address",
                        hex(slot),
                        "--expected-udid",
                        udid,
                        "--output",
                        str(artifact_path),
                    ],
                    env,
                )
            except CommandFailure as error:
                if error.code == 8 and attempt < len(slots):
                    print(f"slot {slot:#x} occupied; trying another address")
                    continue
                raise
        write_new(request_path, attempt_request.read_bytes())
        selected_slot = slot
        break
    if selected_slot is None:
        raise RuntimeError("no usable side-slot address")
    artifact = artifact_path.read_bytes()
    if len(artifact) != 920 or artifact[:0x160] != b"Z" * 0x160:
        raise ValueError("hardware result is malformed")
    sidewrite = artifact[-16:]

    core3 = core3__derive(sinf, sidewrite)
    output = run(
        [
            sys.executable,
            str(ROOT / "core4.py"),
            "--license-sinf",
            str(sinf_path),
            "--hardware-sidewrite-hex",
            sidewrite.hex(),
        ],
        env,
        compact=True,
    )
    matches = CONTEXT_RE.findall(output)
    if len(matches) != 1 or "returned after " not in output:
        raise ValueError("core-4 did not produce exactly one key context")
    source = aes__core_decrypt(core3, bytes.fromhex(matches[0]))
    table = tail__table_from_static_init((ROOT / "fairplayd.A2.ios14").read_bytes())
    credential = tail__final_pass(tail__table_pass(tail__first_pass(source), table))
    if len(credential) != 16:
        raise ValueError("derived credential must contain 16 bytes")
    derive_work16(credential, sinf)
    write_new(credential_path, credential)
    manifest = {
        "uid899_sha256": digest(uid),
        "request_sha256": digest(request_path.read_bytes()),
        "source16_sha256": digest(source16),
        "hardware_sha256": digest(artifact),
        "credential_sha256": digest(credential),
        "sidb_sha256": digest(sidb),
        "sinf_sha256": digest(sinf),
        "sinf_member": sinf_label,
        "device_udid": udid,
        "slot_address": hex(selected_slot),
        "build": SUPPORTED_BUILD,
        "product_type": product_type,
        "used_saved_uid_checkpoint": bool(args.uid_key_file),
        "used_saved_hardware_checkpoint": bool(args.hardware_artifact),
    }
    write_new(
        args.output_dir / "manifest.json",
        (json.dumps(manifest, indent=2) + "\n").encode(),
    )
    print(f"credential ready: {credential_path}")


if __name__ == "__main__":
    main()
