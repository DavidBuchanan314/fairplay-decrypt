#!/usr/bin/env python3
"""Decrypt an authorized IPA using a 16-byte credential."""

import argparse
import hashlib
import os
from pathlib import Path
import tempfile
from zipfile import ZipFile

from core import BlockTransform, arm64_encryption_info, decrypt_binary

ROOT = Path(__file__).resolve().parent


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("ipa", type=Path, help="authorized encrypted IPA")
    parser.add_argument(
        "output", type=Path, help="decrypted IPA output; must not exist"
    )
    parser.add_argument(
        "--credential",
        type=Path,
        default=ROOT / "credential.bin",
        help="saved 16-byte credential",
    )
    args = parser.parse_args()
    if not args.ipa.is_file() or not args.credential.is_file():
        parser.error("input IPA or credential file missing")
    if args.output.exists():
        parser.error("output already exists; never overwrite it")
    if args.output.resolve() == args.ipa.resolve():
        parser.error("input and output must differ")
    credential = args.credential.read_bytes()
    if len(credential) != 16:
        parser.error("credential must contain exactly 16 bytes")
    source_hash = sha256_file(args.ipa)
    with BlockTransform(
        ROOT / "transform", ROOT / "transform_const_data.bin"
    ) as transform, ZipFile(args.ipa) as archive:
        members = archive.namelist()
        names = set(members)
        if len(names) != len(members):
            raise ValueError("input ZIP has duplicate member names")
        plan = []
        skipped_clear = []
        for supf_path in sorted(
            name
            for name in names
            if name.endswith(".supf") and not name.endswith(".v5.supf")
        ):
            parts = supf_path.rsplit("/SC_Info/", 1)
            if len(parts) != 2:
                raise ValueError(f"SUPF outside SC_Info: {supf_path}")
            bundle, filename = parts
            stem = filename.removesuffix(".supf")
            binary_path = bundle + "/" + stem
            sinf_path = bundle + "/SC_Info/" + stem + ".sinf"
            if binary_path not in names or sinf_path not in names:
                raise ValueError(f"SUPF lacks binary/SINF: {supf_path}")
            source = archive.read(binary_path)
            try:
                info = arm64_encryption_info(source)
            except ValueError as error:
                if str(error).startswith("unsupported cryptid/size: 0,"):
                    skipped_clear.append(binary_path)
                    continue
                raise
            plan.append(
                (
                    binary_path,
                    source,
                    archive.read(sinf_path),
                    archive.read(supf_path),
                    info,
                )
            )
        if not plan:
            raise ValueError("no encrypted arm64 binaries with supported SINF/SUPF")
        print(
            f"source-sha256={source_hash} encrypted={len(plan)} "
            f"already-clear={len(skipped_clear)}",
            flush=True,
        )
        replacements = {}
        total_pages = 0
        for ordinal, (binary_path, source, sinf, supf, info) in enumerate(plan, 1):
            result, pages = decrypt_binary(source, sinf, supf, credential, transform)
            if len(result) != len(source):
                raise AssertionError("decrypted Mach-O changed size")
            start = info.base + info.cryptoff
            end = start + info.cryptsize
            if (
                result[: info.cryptid_offset] != source[: info.cryptid_offset]
                or result[info.cryptid_offset + 4 : start]
                != source[info.cryptid_offset + 4 : start]
                or result[end:] != source[end:]
            ):
                raise AssertionError("bytes outside cryptid/encrypted region changed")
            digest = hashlib.sha256(result).hexdigest()
            replacements[binary_path] = result
            total_pages += pages
            print(
                f"{ordinal:02d}/{len(plan)} pages={pages} sha256={digest} "
                f"{binary_path}",
                flush=True,
            )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(
            prefix="fairplay-linux-output-", dir=args.output.parent
        ) as temp:
            provisional = Path(temp) / "provisional.ipa"
            with ZipFile(provisional, "w") as output:
                for entry in archive.infolist():
                    data = (
                        replacements[entry.filename]
                        if entry.filename in replacements
                        else archive.read(entry.filename)
                    )
                    output.writestr(entry, data)
            with ZipFile(provisional) as check:
                if check.testzip() is not None:
                    raise ValueError("output ZIP CRC validation failed")
                for name, expected in replacements.items():
                    if check.read(name) != expected:
                        raise ValueError(f"output ZIP mismatch: {name}")
            os.link(provisional, args.output)
    print(
        f"decrypted-binaries={len(replacements)} pages={total_pages} "
        f"output={args.output} sha256={sha256_file(args.output)}"
    )


if __name__ == "__main__":
    main()
