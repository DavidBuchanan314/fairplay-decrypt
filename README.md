# IPA decryptor

Decrypt FairPlay-encrypted apps using a 16-byte credential
for the account that owns each IPA. [Credential retrieval](credential-retrieval/README.md)
gets it from a jailbroken iOS/iPadOS 14.7.1 (18G82) device and writes
`credential.bin` to the chosen output directory.

Decryption runs on macOS and Linux.

Supports version-3 SUPF (`\x03` `510` or `\x03` `612`), 448-byte SINF `priv`
records, and encrypted arm64 Mach-O slices. It does not support the iOS 26+
license format.

## Set up

From this directory, with Python 3.10+ and a C compiler:

```sh
python3 -m venv .venv
. .venv/bin/activate
python -m pip install -r requirements.txt
./build.sh
```

`build.sh` compiles `transform.c` and checks its output.

## Decrypt

```sh
python decrypt.py /path/to/encrypted.ipa /path/to/decrypted.ipa \
  --credential /path/to/credential.bin
```

If `--credential` is omitted, the tool reads `./credential.bin`. The output
path must not exist.

The decryptor uses each IPA's SINF and SUPF records to derive page keys,
decrypt its encrypted arm64 pages, and clear `cryptid`. It validates the
resulting ZIP.
