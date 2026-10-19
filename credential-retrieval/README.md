# Credential retrieval: iOS/iPadOS 14.7.1 (18G82)

Retrieve a 16-byte credential for an authorized IPA on a jailbroken
iOS/iPadOS 14.7.1 (18G82) device. This path is build-specific and does not
support the iOS 26+ license format.

## Live retrieval

The device must be paired over USB, signed into the account that owns the
IPA, and running Frida with kernel-read support. Connect one device. On the
host, install Python 3.10+ and `idevice_id`/`ideviceinfo`, then run from the
project root:

```sh
python3 -m venv .venv
. .venv/bin/activate
python -m pip install -r requirements.txt -r credential-retrieval/requirements.txt
python credential-retrieval/retrieve.py --preflight
python credential-retrieval/retrieve.py \
  --ipa /path/to/authorized.ipa \
  --output-dir /path/to/a-new-run
```

Use `--sinf /path/to/license.sinf` instead of `--ipa` for a raw license. If
an IPA has distinct license selectors, add `--sinf-member` to choose one.
By default, the tool reads `IC-Info.sidb` from the device; `--sidb` accepts a
local copy. It checks the device build, generates the selector request, calls
the entitled helper, and verifies the credential against the license.

The output directory must not exist. It contains the SIDB, SINF, device key,
hardware result, `credential.bin`, and run manifest. Pass `credential.bin` to
`decrypt.py` with `--credential`.

## Offline verification

To repeat the software derivation with files from a previous run:

```sh
python credential-retrieval/retrieve.py \
  --sidb /path/to/IC-Info.sidb \
  --sinf /path/to/license.sinf \
  --device-udid 40_HEX_DIGITS_FROM_THAT_DEVICE \
  --uid-key-file /path/to/uid899.bin \
  --hardware-artifact /path/to/hardware-result.bin \
  --slot-address 0xADDRESS_FROM_RUN_MANIFEST \
  --output-dir /path/to/a-new-run
```

The address must match the saved hardware result.

## Rebuild the device helper

On a Mac with an iOS SDK and `ldid`:

```sh
xcrun --sdk iphoneos clang -arch arm64 -miphoneos-version-min=14.0 -O2 \
  -Wall -Wextra credential-retrieval/helper.c -framework IOKit \
  -o credential-retrieval/helper.ios14
ldid -Scredential-retrieval/helper.entitlements.plist \
  credential-retrieval/helper.ios14
```

After rebuilding, update the helper hash in `retrieve.py` if it changed.
