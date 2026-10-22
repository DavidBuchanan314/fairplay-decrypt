# Credential retrieval

Retrieve a reusable 16-byte FairPlay account credential from a jailbroken
device. The host needs Python, `idevice_id`, `iproxy`, and USB SSH. The device
runs native programs only; it does not need Python or Frida.

Tested on iOS 14, iOS 16.3.1 (iPhone 11), and iOS 18.

## Retrieve

Connect one paired, jailbroken device with root SSH key access. Install at
least one App Store app on that device. From the project root:

```sh
make
python3 credential-retrieval/retrieve.py --output-dir /path/to/new-output-directory
```

The retriever uses the first installed app with a supported license. To
select a particular account, pass `--ipa /path/to/app.ipa` or
`--sinf /path/to/license.sinf`; an installed app under that account is still
required. If an IPA has distinct license selectors, specify one with
`--sinf-member`. Set `--ssh-port` if the USB SSH tunnel does not use port 2222.
The output directory must not exist; it receives `credential.bin` for use
with `decrypt.py --credential`.

Build on a Mac with Xcode and `ldid`.

## Modern UDIDs

The host now accepts both legacy 40-hex UDIDs and modern `8hex-16hex`
UDIDs. USB uses the original ID. The native helper and host comparison use
FairPlay's internal 20-byte representation. On the tested iPhone 11 with
iOS 16.3.1, this is the byte prefix `6097a4b0c20a0a9f` followed by the
12 hex-decoded bytes of the modern UDID (without the hyphen). This mapping
was verified against the device's `fairplayd.H2` parser at runtime.

## Native breakpoint state refresh

On iOS 16.3.1, an in-place `ARM_DEBUG_STATE64` update could read back the
new register values but still hit the old entry breakpoint on the CPU.
The observer now writes a fully zeroed debug state before installing each
new configuration, including when restoring saved state. The target thread
is stopped during both writes; the final state retains the original debug
slots outside the two used by the observer.

The behavior matches XNU 8792's debug-state caching:

- [`machine_thread_set_state`](https://github.com/apple-oss-distributions/xnu/blob/xnu-8792.81.2/osfmk/arm64/status.c#L1414)
  mutates an existing debug-state object for a remote thread. With all debug
  enables cleared, it releases that object instead.
- [The EL0 return path](https://github.com/apple-oss-distributions/xnu/blob/xnu-8792.81.2/osfmk/arm64/locore.s#L933)
  reloads hardware registers only when the thread's debug-state pointer
  differs from the CPU's cached pointer. Clearing and reinstalling avoids
  relying on an in-place update being noticed.

Validation on the connected iPhone 11: native-only credential retrieval
succeeded twice with identical credentials; the credential also passed
SINF `priv` decoding. This does not establish that
every OS version or scheduling configuration exhibits the original issue.
