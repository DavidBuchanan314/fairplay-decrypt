"""Offline credential-derivation primitives for build 18G82."""

from __future__ import annotations
from pathlib import Path
import struct
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import hashlib
import os
from unicorn import Uc, UcError, UC_ARCH_ARM64, UC_MODE_ARM
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn import arm64_const
from dataclasses import dataclass
from functools import lru_cache


def runtime__path(variable: str) -> Path:
    value = os.environ.get(variable)
    if not value:
        raise ValueError(f"{variable} is required for this retrieval run")
    return Path(value)


def runtime__udid() -> bytes:
    value = os.environ.get("FAIRPLAY_DEVICE_UDID")
    if not value:
        raise ValueError("FAIRPLAY_DEVICE_UDID is required for this retrieval run")
    try:
        udid = bytes.fromhex(value)
    except ValueError as error:
        raise ValueError("device UDID is not hexadecimal") from error
    if len(udid) != 20:
        raise ValueError("device UDID must contain 20 bytes")
    return udid


def macho__segments(data: bytes):
    if struct.unpack_from("<I", data)[0] != 4277009103:
        raise ValueError("not a little-endian Mach-O 64 image")
    ncmds = struct.unpack_from("<I", data, 16)[0]
    offset = 32
    result = []
    for _ in range(ncmds):
        cmd, cmdsize = struct.unpack_from("<II", data, offset)
        if cmd == 25:
            name = data[offset + 8 : offset + 24].split(b"\x00", 1)[0].decode()
            vmaddr, vmsize, fileoff, filesize = struct.unpack_from(
                "<QQQQ", data, offset + 24
            )
            result.append((name, vmaddr, vmsize, fileoff, filesize))
        offset += cmdsize
    return result


def macho__file_offset(segments, address: int) -> int:
    for _, vmaddr, _, fileoff, filesize in segments:
        if vmaddr <= address < vmaddr + filesize:
            return fileoff + address - vmaddr
    raise ValueError(f"address outside saved file-backed segments: {address:#x}")


def rebases__uleb(data: bytes, position: int) -> tuple[int, int]:
    value = shift = 0
    while True:
        if position >= len(data):
            raise ValueError("truncated ULEB128")
        byte = data[position]
        position += 1
        value |= (byte & 127) << shift
        if not byte & 128:
            return (value, position)
        shift += 7
        if shift >= 64:
            raise ValueError("ULEB128 overflow")


def rebases__rebase_addresses(image: bytes) -> list[int]:
    if struct.unpack_from("<I", image)[0] != 4277009103:
        raise ValueError("not a little-endian Mach-O 64 image")
    ncmds = struct.unpack_from("<I", image, 16)[0]
    position = 32
    segments = []
    rebase_info = None
    for _ in range(ncmds):
        cmd, size = struct.unpack_from("<II", image, position)
        if size < 8 or position + size > len(image):
            raise ValueError("invalid load command")
        if cmd == 25:
            segments.append(struct.unpack_from("<QQ", image, position + 24))
        elif cmd in (34, 2147483682):
            rebase_info = struct.unpack_from("<II", image, position + 8)
        position += size
    if rebase_info is None:
        raise ValueError("image has no dyld-info rebase stream")
    offset, size = rebase_info
    stream = image[offset : offset + size]
    if len(stream) != size:
        raise ValueError("truncated rebase stream")
    position = 0
    address = None
    kind = None
    output = []

    def emit() -> None:
        nonlocal address
        if address is None or kind != 1:
            raise ValueError("unsupported or uninitialized rebase")
        output.append(address)
        address += 8

    while position < len(stream):
        instruction = stream[position]
        position += 1
        opcode, immediate = (instruction & 240, instruction & 15)
        if opcode == 0:
            break
        if opcode == 16:
            kind = immediate
        elif opcode == 32:
            if immediate >= len(segments):
                raise ValueError("invalid rebase segment index")
            delta, position = rebases__uleb(stream, position)
            vmaddr, vmsize = segments[immediate]
            if delta >= vmsize:
                raise ValueError("rebase starts outside segment")
            address = vmaddr + delta
        elif opcode == 48:
            delta, position = rebases__uleb(stream, position)
            address += delta
        elif opcode == 64:
            address += immediate * 8
        elif opcode == 80:
            for _ in range(immediate):
                emit()
        elif opcode == 96:
            times, position = rebases__uleb(stream, position)
            for _ in range(times):
                emit()
        elif opcode == 112:
            emit()
            delta, position = rebases__uleb(stream, position)
            address += delta
        elif opcode == 128:
            times, position = rebases__uleb(stream, position)
            skip, position = rebases__uleb(stream, position)
            for _ in range(times):
                emit()
                address += skip
        else:
            raise ValueError(f"unknown rebase opcode {opcode:#x}")
    return output


priv_transform__MASK_OFFSETS = (15752048, 13967152, 14963589)
priv_transform__FINAL_MASK_OFFSETS = (15254240, 15413458, 14472416)


def priv_transform__children(data: bytes, start: int, end: int):
    offset = start
    while offset < end:
        if offset + 8 > end:
            raise ValueError("truncated SINF atom")
        size = int.from_bytes(data[offset : offset + 4], "big")
        if size < 8 or offset + size > end:
            raise ValueError("invalid SINF atom size")
        yield data[offset + 4 : offset + 8], offset + 8, offset + size
        offset += size


def priv_transform__atom(sinf: bytes, name: bytes) -> bytes:
    roots = list(priv_transform__children(sinf, 0, len(sinf)))
    if len(roots) != 1 or roots[0][0] != b"sinf":
        raise ValueError("expected one outer SINF atom")
    schi = [
        item
        for item in priv_transform__children(sinf, roots[0][1], roots[0][2])
        if item[0] == b"schi"
    ]
    if len(schi) != 1:
        raise ValueError("expected one SINF schi container")
    fields = [
        item
        for item in priv_transform__children(sinf, schi[0][1], schi[0][2])
        if item[0] == name
    ]
    if len(fields) != 1:
        raise ValueError(f"expected one SINF {name!r} field")
    return sinf[fields[0][1] : fields[0][2]]


def priv_transform__transform_stage1(priv: bytes, daemon: bytes) -> bytes:
    masks = [daemon[offset : offset + 16] for offset in priv_transform__MASK_OFFSETS]
    if any((len(mask) != 16 for mask in masks)):
        raise ValueError("daemon binary is too short for masks")
    return bytes(
        (
            value
            ^ masks[0][i & 15]
            ^ masks[1][i & 15]
            ^ masks[2][i & 15]
            ^ (i & 15) * 79 & 255
            for (i, value) in enumerate(priv)
        )
    )


def priv_transform__transform_final(middle: bytes, daemon: bytes) -> bytes:
    """Apply the second fixed-byte pass; `middle` is still an unknown output."""
    masks = [
        daemon[offset : offset + 16] for offset in priv_transform__FINAL_MASK_OFFSETS
    ]
    if any((len(mask) != 16 for mask in masks)):
        raise ValueError("daemon binary is too short for final masks")
    return bytes(
        (
            value
            ^ masks[0][i & 15]
            ^ masks[1][i & 15]
            ^ masks[2][i & 15]
            ^ (i & 15) * 15 & 255
            ^ 48
            for (i, value) in enumerate(middle)
        )
    )


def aes__xor30(data: bytes) -> bytes:
    return bytes((value ^ 48 for value in data))


def aes__core_decrypt(data: bytes, key: bytes) -> bytes:
    if len(data) != 16 or len(key) != 16:
        raise ValueError("core block and key must each be 16 bytes")
    cipher = Cipher(algorithms.AES(aes__xor30(key)), modes.ECB())
    op = cipher.decryptor()
    return aes__xor30(op.update(aes__xor30(data)) + op.finalize())


tail__ROOT = Path(__file__).resolve().parent


def tail__table_from_static_init(daemon: bytes) -> bytes:
    """Load the static table matching the bundled fairplayd image."""
    if (
        hashlib.sha256(daemon).hexdigest()
        != "30469ad8d24c232c277af6ce5a794b1fb36b4f25543b642be331d3b3f6b5d411"
    ):
        raise ValueError("static table belongs to a different fairplayd image")
    table = (tail__ROOT / "static-key-table.bin").read_bytes()
    if (
        len(table) != 4096
        or hashlib.sha256(table).hexdigest()
        != "c7b33507f71d5f7a07f623eb21f6b9f23997f950119371631f6535f63e6d2af1"
    ):
        raise ValueError("static table is missing or altered")
    return table


def tail__first_pass(source: bytes) -> bytes:
    if len(source) != 16:
        raise ValueError("expected 16-byte source")
    return bytes(((value ^ 48) * -13 - 111 & 255 for value in source))


def tail__table_pass(source: bytes, table: bytes) -> bytes:
    if len(source) != 16 or len(table) != 4096:
        raise ValueError("expected 16-byte source and 4096-byte table")
    return bytes(
        (
            table[i * 256 + (value * 59 - 107 & 255)] * -13 - 111 & 255
            for (i, value) in enumerate(source)
        )
    )


def tail__final_pass(source: bytes) -> bytes:
    if len(source) != 16:
        raise ValueError("expected 16-byte source")
    return bytes((value * 59 - (value * 118 + 42 & 96) - 59 & 255 for value in source))


uid__ROOT = Path(__file__).resolve().parent
uid__DAEMON = uid__ROOT / "fairplayd.A2.ios14"
uid__STATIC_IV_OFFSET = 15455472


def uid__derive_iv() -> bytes:
    encoded = uid__DAEMON.read_bytes()[
        uid__STATIC_IV_OFFSET : uid__STATIC_IV_OFFSET + 16
    ]
    if len(encoded) != 16:
        raise ValueError("daemon binary does not contain the first-layer IV")
    return bytes((value ^ 48 for value in encoded))


uid__IV = uid__derive_iv()


def uid__derive(uid_key_file: Path | None = None) -> bytes:
    sidb = runtime__path("FAIRPLAY_SIDB_FILE").read_bytes()
    if uid_key_file is None:
        override = os.environ.get("FAIRPLAY_UID899_KEY_FILE")
        if not override:
            raise ValueError("a fresh UID 0x899 key is required")
        uid_key_file = Path(override)
    key = uid_key_file.read_bytes()
    if (
        len(sidb) < 36
        or (len(sidb) - 20) % 16
        or sidb[:4] != bytes.fromhex("80060003")
        or len(key) != 16
    ):
        raise ValueError("unexpected SIDB version, size, or 0x899 key length")
    op = Cipher(algorithms.AES(key), modes.CBC(uid__IV)).decryptor()
    return op.update(sidb[4:-16]) + op.finalize()


keybag_layers__ROOT = Path(__file__).resolve().parent
keybag_layers__DAEMON = keybag_layers__ROOT / "fairplayd.A2.ios14"
keybag_layers__STATIC_IV_OFFSET = 15317472


def keybag_layers__xor30(data):
    return bytes((value ^ 48 for value in data))


def keybag_layers__derive_keybag(descriptor_key: bytes):
    if len(descriptor_key) != 16:
        raise ValueError("descriptor key must be exactly 16 bytes")
    daemon = keybag_layers__DAEMON.read_bytes()
    iv = daemon[keybag_layers__STATIC_IV_OFFSET : keybag_layers__STATIC_IV_OFFSET + 16]
    if len(iv) != 16:
        raise ValueError("daemon binary does not contain second-layer IV")
    uid_plaintext = uid__derive()
    stage1 = priv_transform__transform_stage1(uid_plaintext, daemon)
    context = Cipher(
        algorithms.AES(keybag_layers__xor30(descriptor_key)), modes.CBC(iv)
    ).decryptor()
    clear = context.update(uid_plaintext) + context.finalize()
    encoded = keybag_layers__xor30(clear)
    middle = priv_transform__transform_final(encoded, daemon)
    return (uid_plaintext, stage1, middle, encoded, clear)


descriptor_pre__MASK32 = 4294967295
descriptor_pre__SITE_COMMON = "0x21d748"
descriptor_pre__SITE_A = ("0x21d5e0", "0x21d608")
descriptor_pre__SITE_B = ("0x21d830", "0x21d854")


def descriptor_pre__u32(value: int) -> int:
    return value & descriptor_pre__MASK32


def descriptor_pre__simulate(
    image: bytes, table: bytes
) -> tuple[list[tuple[str, int, int]], bytes]:
    if len(table) != 20:
        raise ValueError("descriptor lookup must be 20 bytes")
    state = bytearray(image[14324968:14325016] + image[15939424:15939440])
    if len(state) != 64:
        raise ValueError("daemon template incomplete")
    a = image[14590755:14590771]
    b = image[15950372:15950388]
    c = image[14225808:14225824]
    outputs: list[tuple[str, int, int]] = []
    constant = 491398319
    for step in range(20):
        lane = step & 15
        t0 = table[step % 20]
        source = state[(step + 1) % 48] ^ 46
        t1 = table[(table[source % 20] + step) % 20]
        raw = a[lane] ^ lane * 121 ^ t0 ^ t1 ^ state[48 + lane] ^ b[lane] ^ c[lane]
        mix = a[lane] ^ lane * 121 ^ b[lane] ^ c[lane]
        first_tail = (mix ^ raw) & 255
        outputs.append((descriptor_pre__SITE_COMMON, 48 + lane, first_tail))
        state[48 + lane] = first_tail
        control_word = descriptor_pre__u32(descriptor_pre__u32(raw & 255) + constant)
        control_word = descriptor_pre__u32(control_word - (350 & (raw & 255) << 1))
        control = control_word ^ constant
        prior = state[control % 48]
        branch_a = raw & 255 <= prior ^ 46
        prior_first = state[step % 48]
        if branch_a:
            v1 = state[descriptor_pre__u32(constant + 59 ^ control_word) % 48]
            idx = descriptor_pre__u32(constant - 131 ^ control_word) % 20
            v2 = table[idx]
            v2 = descriptor_pre__u32(v2 - (92 & v2 << 1) + 46)
            idx = (
                descriptor_pre__u32(
                    descriptor_pre__u32(constant - 121 ^ control_word) + step
                )
                % 20
            )
            v3 = table[idx]
            v3 = state[v3 % 48] ^ 46
            v3 = state[v3 % 48] ^ 46
            v3 = table[v3 % 20]
            x = descriptor_pre__u32(raw ^ 40 ^ v1 ^ v2 ^ v3 ^ 75)
            new_tail = (mix ^ x ^ 99) & 255
            new_first = (x & 171 ^ prior_first ^ 35) & 255
            tail_site, first_site = descriptor_pre__SITE_A
        else:
            v1 = table[control % 20]
            v1 = descriptor_pre__u32(v1 - (92 & v1 << 1) + 46)
            v2 = table[descriptor_pre__u32(control + step) % 20]
            v2 = state[v2 % 48] ^ 46
            v2 = state[v2 % 48] ^ 46
            v2 = table[v2 % 20]
            x = descriptor_pre__u32(prior ^ (raw ^ 40) ^ v1 ^ v2 ^ 87)
            new_tail = (mix ^ x ^ 127) & 255
            nibble_swap = (x << 4 | x >> 4 & 15) & 255
            new_first = (prior_first ^ nibble_swap ^ 247) & 255
            tail_site, first_site = descriptor_pre__SITE_B
        outputs.append((tail_site, 48 + lane, new_tail))
        state[48 + lane] = new_tail
        outputs.append((first_site, step % 48, new_first))
        state[step % 48] = new_first
    return (outputs, bytes(state))


descriptor_rec__MASK32 = 4294967295


def descriptor_rec__tables(image: bytes) -> tuple[bytes, bytes, bytes]:
    return (
        image[14225808:14225824],
        image[14590755:14590771],
        image[15950372:15950388],
    )


def descriptor_rec__simulate(initial: bytes, image: bytes) -> tuple[bytes, list[bytes]]:
    if len(initial) != 64:
        raise ValueError("initial descriptor mix buffer must be exactly 64 bytes")
    a, b, c = descriptor_rec__tables(image)
    if any((len(table) != 16 for table in (a, b, c))):
        raise ValueError("daemon image lacks descriptor-mix tables")
    state = bytearray(initial)
    states = []
    outputs = bytearray()
    for step in range(64):
        states.append(bytes(state))
        lane = step & 15
        mixed = state[48 + lane] ^ a[lane] ^ b[lane] ^ c[lane] ^ lane * 121 & 255
        control = (mixed ^ 1068482492) + (376 & mixed << 1) & descriptor_rec__MASK32
        select = control + 4 & 15
        source_index = (control + 3226484804 & descriptor_rec__MASK32) % 48
        output = (
            state[48 + select]
            ^ state[source_index]
            ^ a[select]
            ^ b[select]
            ^ c[select]
            ^ select * 121 & 255
            ^ 30
        ) & 255
        outputs.append(output)
        first_index = step % 48
        old = state[first_index]
        select = old & 15 ^ 14
        new_first = (
            state[48 + select]
            ^ old
            ^ (mixed & 240) >> 4
            ^ a[select]
            ^ b[select]
            ^ c[select]
            ^ select * 121 & 255
        ) & 255
        state[first_index] = new_first
        select = new_first & 15 ^ 14
        candidate = (
            state[48 + select] ^ a[select] ^ b[select] ^ c[select] ^ select * 121 & 255
        ) & 255
        state[48 + lane] = (state[candidate % 48] ^ state[48 + lane] ^ 46) & 255
    return (bytes(outputs), states)


descriptor__IMAGE_BASE = 4294967296
descriptor__STACK_BASE = 481036337152
descriptor__CANARY_BASE = 485331304448
descriptor__FUNCTION_ENTRY = 5959200
descriptor__FINALIZER_END = 5981876
descriptor__DESCRIPTOR_OFFSET = 60800
descriptor__SEED_OFFSET = 43200


def descriptor__finish_seed(image: bytes, initial: bytes) -> bytes:
    """Run the daemon's original finalizer on a synthetic descriptor."""
    if len(initial) != 64:
        raise ValueError("pre-finalizer seed must be 64 bytes")
    engine = Uc(UC_ARCH_ARM64, UC_MODE_ARM)
    engine.mem_map(descriptor__IMAGE_BASE, 16646144)
    for name, address, vmsize, fileoff, filesize in macho__segments(image):
        if name == "__PAGEZERO" or vmsize == 0:
            continue
        data = (
            bytes(vmsize) if name == "__DATA" else image[fileoff : fileoff + filesize]
        )
        if data:
            engine.mem_write(address, data)
    engine.mem_map(descriptor__STACK_BASE, 65536)
    engine.mem_map(descriptor__CANARY_BASE, 4096)
    engine.mem_write(
        descriptor__IMAGE_BASE + 16056336, descriptor__CANARY_BASE.to_bytes(8, "little")
    )
    engine.mem_write(descriptor__CANARY_BASE, bytes(8))
    descriptor = descriptor__STACK_BASE + descriptor__DESCRIPTOR_OFFSET
    seed = descriptor__STACK_BASE + descriptor__SEED_OFFSET
    tail = seed + 48
    engine.mem_write(seed, initial)
    engine.mem_write(descriptor + 8, tail.to_bytes(8, "little") * 2)
    encoded_index = (
        5 - ((descriptor & 4294967295 ^ 2080505244) * 1166739311 & 4294967295)
        & 4294967295
    )
    engine.mem_write(descriptor + 24, encoded_index.to_bytes(4, "little"))
    engine.reg_write(arm64_const.UC_ARM64_REG_X0, descriptor)
    engine.reg_write(arm64_const.UC_ARM64_REG_SP, descriptor__STACK_BASE + 144)
    engine.reg_write(
        arm64_const.UC_ARM64_REG_PC, descriptor__IMAGE_BASE + descriptor__FUNCTION_ENTRY
    )
    result = {"steps": 0, "outcome": "instruction limit"}

    def on_code(uc: Uc, address: int, size: int, user: object) -> None:
        result["steps"] += 1
        if address == descriptor__IMAGE_BASE + descriptor__FINALIZER_END:
            result["outcome"] = "reached finalizer end"
            uc.emu_stop()
        elif result["steps"] >= 100000:
            uc.emu_stop()

    def on_invalid(
        uc: Uc, access: int, address: int, size: int, value: int, user: object
    ) -> bool:
        pc = uc.reg_read(arm64_const.UC_ARM64_REG_PC)
        result["outcome"] = (
            f"unmapped access type={access} at {address:#x}, size={size}, pc={pc - descriptor__IMAGE_BASE:#x}"
        )
        return False

    engine.hook_add(UC_HOOK_CODE, on_code)
    engine.hook_add(UC_HOOK_MEM_INVALID, on_invalid)
    try:
        engine.emu_start(
            descriptor__IMAGE_BASE + descriptor__FUNCTION_ENTRY, 0, count=100000
        )
    except UcError as exc:
        raise RuntimeError(f"finalizer emulation failed: {result['outcome']}") from exc
    if result["outcome"] != "reached finalizer end":
        raise RuntimeError(f"finalizer emulation stopped: {result['outcome']}")
    if result["steps"] < 1000:
        raise AssertionError("finalizer path unexpectedly short")
    output = bytes(engine.mem_read(seed, 64))
    if output[:48] != initial[:48]:
        raise AssertionError("finalizer unexpectedly altered first 48 bytes")
    return output


def descriptor__derive(image: bytes, udid: bytes) -> tuple[bytes, bytes, bytes]:
    if len(udid) != 20:
        raise ValueError("saved device UDID must be 20 raw bytes")
    _, before = descriptor_pre__simulate(image, udid)
    after = descriptor__finish_seed(image, before)
    outputs, _ = descriptor_rec__simulate(after, image)
    return (before, after, outputs[21:37])


atoms__ROOT = Path(__file__).resolve().parent
atoms__CONTAINERS = {b"sean", b"grup", b"head", b"user", b"key ", b"tail"}


@lru_cache(maxsize=1)
def atoms__saved_descriptor_key() -> bytes:
    """Derive the SIDB descriptor key from this run's device UDID."""
    image = (atoms__ROOT / "fairplayd.A2.ios14").read_bytes()
    return descriptor__derive(image, runtime__udid())[2]


@dataclass
class atoms__Atom:
    tag: str
    start: int
    size: int
    meta: bytes
    payload: bytes
    children: list[atoms__Atom]


def atoms__parse_atoms(data: bytes, start: int, end: int) -> list[atoms__Atom]:
    result = []
    at = start
    while at < end:
        if at + 20 > end:
            raise ValueError(f"truncated atom header at {at:#x}")
        size = int.from_bytes(data[at : at + 4], "big")
        tag = data[at + 4 : at + 8]
        if size < 20 or at + size > end or (not all((32 <= x < 127 for x in tag))):
            raise ValueError(f"invalid atom at {at:#x}: size={size} tag={tag!r}")
        payload = data[at + 20 : at + size]
        children = (
            atoms__parse_atoms(data, at + 20, at + size)
            if tag in atoms__CONTAINERS
            else []
        )
        result.append(
            atoms__Atom(
                tag.decode("ascii"), at, size, data[at + 8 : at + 20], payload, children
            )
        )
        at += size
    return result


def atoms__descendants(nodes, tag):
    for node in nodes:
        if node.tag == tag:
            yield node
        yield from atoms__descendants(node.children, tag)


def atoms__users(clear: bytes):
    if len(clear) < 32 or clear[:12] != bytes(12):
        raise ValueError("unexpected clear SIDB wrapper")
    size = int.from_bytes(clear[12:16], "big")
    if size < 20 or 12 + size > len(clear):
        raise ValueError("invalid clear SIDB atom size")
    return list(atoms__descendants(atoms__parse_atoms(clear, 12, 12 + size), "user"))


def atoms__keybag_value_for_sinf(users, sinf):
    account = priv_transform__atom(sinf, b"user")
    key_index_raw = priv_transform__atom(sinf, b"key ")
    if len(account) != 4 or len(key_index_raw) != 4:
        raise ValueError("unexpected SINF user/key-index size")
    key_index = int.from_bytes(key_index_raw, "big")
    matching_users = [user for user in users if user.meta[:4] == account]
    if len(matching_users) != 1:
        raise ValueError(f"keybag user lookup count={len(matching_users)}")
    matching_keys = [
        item
        for item in atoms__descendants(matching_users, "key ")
        if int.from_bytes(item.meta[:4], "big") == key_index
    ]
    if len(matching_keys) != 1:
        raise ValueError(f"keybag key-index lookup count={len(matching_keys)}")
    values = list(atoms__descendants(matching_keys, "valu"))
    if len(values) != 1 or len(values[0].payload) != 16:
        raise ValueError("keybag value missing or malformed")
    return values[0].payload


def source__derive_source16(sinf: bytes) -> bytes:
    """Select the account's clear SIDB session and return its first block."""
    account = priv_transform__atom(sinf, b"user")
    key_index_raw = priv_transform__atom(sinf, b"key ")
    if len(account) != 4 or len(key_index_raw) != 4:
        raise ValueError("SINF account/key-index field has unexpected size")
    key_index = int.from_bytes(key_index_raw, "big")
    clear = keybag_layers__derive_keybag(atoms__saved_descriptor_key())[-1]
    users = atoms__users(clear)
    matching = [node for node in users if node.meta[:4] == account]
    if len(matching) != 1:
        raise ValueError(
            f"expected one SIDB user for SINF account, got {len(matching)}"
        )
    keys = [
        node
        for node in atoms__descendants([matching[0]], "key ")
        if int.from_bytes(node.meta[:4], "big") == key_index
    ]
    if len(keys) != 1:
        raise ValueError(f"expected one SIDB key for SINF index, got {len(keys)}")
    sessions = list(atoms__descendants([matching[0]], "sess"))
    if len(sessions) != 1 or len(sessions[0].payload) != 256:
        raise ValueError("selected SIDB session missing or malformed")
    return sessions[0].payload[:16]


def core3__derive(sinf: bytes, hardware_output: bytes) -> bytes:
    if len(hardware_output) != 16:
        raise ValueError("FairPlayIOKit side-write must be exactly 16 bytes")
    if len(source__derive_source16(sinf)) != 16:
        raise AssertionError("invalid selected session prefix")
    clear = keybag_layers__derive_keybag(atoms__saved_descriptor_key())[-1]
    users = atoms__users(clear)
    encoded_value = atoms__keybag_value_for_sinf(users, sinf)
    return aes__core_decrypt(
        keybag_layers__xor30(encoded_value), keybag_layers__xor30(hardware_output)
    )


hardware__ROOT = Path(__file__).resolve().parent
hardware__DAEMON = hardware__ROOT / "fairplayd.A2.ios14"
hardware__IV_OFFSET = 14334672
hardware__KEY_MASK = 3000
hardware__KEY_COMMAND = 403243008


def hardware__derive(sinf: bytes) -> dict[str, bytes | int]:
    account = priv_transform__atom(sinf, b"user")
    key_index = priv_transform__atom(sinf, b"key ")
    if len(account) != 4 or len(key_index) != 4:
        raise ValueError("SINF account/key index must each have four bytes")
    clear = keybag_layers__derive_keybag(atoms__saved_descriptor_key())[-1]
    users = atoms__users(clear)
    matching = [user for user in users if user.meta[:4] == account]
    if len(matching) != 1:
        raise ValueError("SINF account does not select exactly one SIDB user")
    selected = matching[0]
    indexes = [
        node
        for node in atoms__descendants([selected], "key ")
        if node.meta[:4] == key_index
    ]
    if len(indexes) != 1:
        raise ValueError("SINF key index does not select exactly one SIDB key")
    scra = list(atoms__descendants([selected], "scra"))
    sess = list(atoms__descendants([selected], "sess"))
    if len(scra) != 1 or len(scra[0].payload) != 49:
        raise ValueError("selected SIDB user lacks one 49-byte scra")
    if len(sess) != 1 or len(sess[0].payload) != 256:
        raise ValueError("selected SIDB user lacks one 256-byte sess")
    image = hardware__DAEMON.read_bytes()
    iv_encoded = image[hardware__IV_OFFSET : hardware__IV_OFFSET + 16]
    if len(iv_encoded) != 16:
        raise ValueError("static daemon IV is truncated")
    return {
        "account": account,
        "key_index": int.from_bytes(key_index, "big"),
        "source16": sess[0].payload[:16],
        "custom_input16": scra[0].payload[:16],
        "custom_key32": scra[0].payload[16:48],
        "iv16": bytes((value ^ 48 for value in iv_encoded)),
        "key_mask": hardware__KEY_MASK,
        "key_command": hardware__KEY_COMMAND,
    }


body__ROOT = Path(__file__).resolve().parent
body__DAEMON = body__ROOT / "fairplayd.A2.ios14"
body__PREMASK_OFFSET = 13393008
body__CONTROL_A = bytes.fromhex("00fb1958000000040000000100fb1958")
body__CONTROL_B = bytes.fromhex("0000000400000000005fc08800000031")
body__CONTROL_POST_SCRA = bytes.fromhex("005fc08800000010")
body__CONTROL_AFTER_IV = bytes.fromhex("00fb1958000000")
body__CONTROL_PTR_PREFIX = bytes.fromhex("08000000")
body__CONTROL_FINAL = bytes.fromhex("04000000108000000000000000000000")


def body__pointer5(address: int) -> bytes:
    if not 0 <= address < 1 << 40:
        raise ValueError("synthetic encoder pointer must fit five bytes")
    return address.to_bytes(5, "big")


def body__derive_body(sinf: bytes, body_address: int) -> bytes:
    fields = hardware__derive(sinf)
    scra = fields["custom_input16"] + fields["custom_key32"]
    clear = keybag_layers__derive_keybag(atoms__saved_descriptor_key())[-1]
    users = atoms__users(clear)
    selected = [
        user for user in users if user.meta[:4] == priv_transform__atom(sinf, b"user")
    ]
    if len(selected) != 1:
        raise ValueError("SINF account does not identify one SIDB user")
    full_scra = list(atoms__descendants(selected, "scra"))
    if (
        len(full_scra) != 1
        or len(full_scra[0].payload) != 49
        or full_scra[0].payload[:48] != scra
    ):
        raise AssertionError("selected 49-byte scra differs from hardware fields")
    scra = full_scra[0].payload
    iv = fields["iv16"]
    plain = (
        body__CONTROL_A
        + body__CONTROL_B
        + scra
        + body__CONTROL_POST_SCRA
        + iv
        + body__CONTROL_AFTER_IV
        + body__CONTROL_PTR_PREFIX
        + body__pointer5(body_address - 319)
        + body__CONTROL_AFTER_IV
        + body__CONTROL_PTR_PREFIX
        + body__pointer5(body_address - 384)
        + body__CONTROL_AFTER_IV
        + body__CONTROL_FINAL
    )
    if len(plain) != 160:
        raise AssertionError(f"middle-encoder plaintext length={len(plain)}")
    image = body__DAEMON.read_bytes()
    mask = image[body__PREMASK_OFFSET : body__PREMASK_OFFSET + 16]
    if mask != bytes.fromhex("01fc9cbfc002df61a1644ef25792d8eb"):
        raise AssertionError("static daemon pre-encoder mask changed")
    return bytes((value ^ mask[index % 16] for (index, value) in enumerate(plain)))


encoder_seed__ROOT = Path(__file__).resolve().parent
encoder_seed__DAEMON = encoder_seed__ROOT / "fairplayd.A2.ios14"
encoder_seed__IMAGE_BASE = 4294967296
encoder_seed__DAEMON_SHA256 = (
    "30469ad8d24c232c277af6ce5a794b1fb36b4f25543b642be331d3b3f6b5d411"
)
encoder_seed__X19 = 13129056
encoder_seed__X20 = 13677024
encoder_seed__X21 = 14740768
encoder_seed__X22 = 15766864


def encoder_seed__derive_encoder_seed(
    daemon_path: Path = encoder_seed__DAEMON,
) -> bytes:
    image = daemon_path.read_bytes()
    actual_hash = hashlib.sha256(image).hexdigest()
    if actual_hash != encoder_seed__DAEMON_SHA256:
        raise ValueError(f"unsupported fairplayd image: SHA-256 {actual_hash}")
    mapping = macho__segments(image)

    def byte_at(offset: int) -> int:
        return image[macho__file_offset(mapping, encoder_seed__IMAGE_BASE + offset)]

    return bytes(
        (
            byte_at(encoder_seed__X21 + index)
            ^ byte_at(encoder_seed__X22 + index)
            ^ byte_at(encoder_seed__X19 + index + 2)
            ^ byte_at(encoder_seed__X20 + index)
            ^ -51 * index & 255
            ^ 48
            for index in range(16)
        )
    )


message__MASK64 = (1 << 64) - 1
message__PROLOGUE_XOR = 8646451110917067333
message__PROLOGUE_MULTIPLIER = 496023503
message__KEY_WORD_XOR = 1433578196
message__USER_WORD_XOR = 16602079


def message__normal(byte: int) -> int:
    return byte - (byte << 1 & 96) + 48 & 255


def message__special(byte: int) -> int:
    mixed = byte ^ 112
    mask = mixed & 60 ^ byte & 12
    return mixed - 2 * mask + 48 & 255


def message__inverse(target: int, xor: int, transform=message__normal) -> int:
    matches = [source for source in range(256) if transform(source ^ xor) == target]
    if len(matches) != 1:
        raise AssertionError(f"byte transform inverse count={len(matches)}")
    return matches[0]


def message__fields(
    udid: bytes, sinf: bytes, descriptor_address: int
) -> dict[str, bytes | int]:
    if len(udid) != 20:
        raise ValueError("device UDID must be 20 bytes")
    user = priv_transform__atom(sinf, b"user")
    index_raw = priv_transform__atom(sinf, b"key ")
    if len(user) != 4 or len(index_raw) != 4:
        raise ValueError("unexpected SINF user/key-index length")
    index = int.from_bytes(index_raw, "big")
    if index > 65535:
        raise ValueError("this daemon path expects a four-digit key index")
    key_ascii = f"{index:04x}".encode("ascii")
    user_xor = bytes((value ^ 48 for value in user))
    key_bytes = bytes(
        (
            message__inverse(key_ascii[0], 151),
            message__inverse(key_ascii[1], 103),
            message__inverse(key_ascii[2], 59),
            message__inverse(key_ascii[3], 85, lambda byte: byte),
        )
    )
    user_bytes = bytes(
        (
            message__inverse(user_xor[0], 159),
            message__inverse(user_xor[1], 0, message__special),
            message__inverse(user_xor[2], 45),
            message__inverse(user_xor[3], 41, lambda byte: byte),
        )
    )
    key_word = int.from_bytes(key_bytes, "big")
    user_word = int.from_bytes(user_bytes, "big")
    mask = (
        descriptor_address ^ message__PROLOGUE_XOR
    ) * message__PROLOGUE_MULTIPLIER & message__MASK64
    field8_low32 = (key_word ^ message__KEY_WORD_XOR) + (mask & 4294967295) & 4294967295
    field40_low32 = (user_word ^ message__USER_WORD_XOR ^ mask) & 4294967295
    message = (
        bytes((message__normal(value) for value in udid[:8])) + key_ascii + user_xor
    )
    return {
        "message": message,
        "dispatch_word": (mask & 4294967295) + 5 & 4294967295,
        "field8_low32": field8_low32,
        "field40_low32": field40_low32,
        "key_word": key_word,
        "user_word": user_word,
    }


control__MASK64 = (1 << 64) - 1
control__IMAGE_BASE = 4294967296
control__XOR1 = 3923296571
control__SUBTRACT_MASK = 5856196848692619710
control__ADD2 = 12151470461201085663
control__XOR3 = 2589806326
control__ADD4 = 610453616
control__NODE_BIAS = 18446744073099098000
control__LATE_STATIC_COMPARE = 2146493716
control__LATE_HELPER_STATE = 222092377


def control__initial_branch_word(
    descriptor_address: int, image: bytes, runtime_base: int
) -> int:
    """Field +0x30 selecting the original +0x2fa2cc control path."""
    mask = (
        descriptor_address ^ message__PROLOGUE_XOR
    ) * message__PROLOGUE_MULTIPLIER & control__MASK64
    raw_base = int.from_bytes(image[16056320 + 164848 : 16056320 + 164856], "little")
    jump_base = raw_base + runtime_base - control__IMAGE_BASE
    target = runtime_base + 3121868
    return target - jump_base + 1425 - mask & 4294967295


def control__late_helper_state(decoded: int, helper_descriptor: int) -> int:
    """Lift daemon +0x2fe498..4fc and helper +0x29ea88..eec."""
    mask32 = 4294967295
    helper_mask = (helper_descriptor & mask32 ^ 2046710341) * 496023503 & mask32
    subtract = 4087325486 + 2 * decoded & 3171535580
    operand = helper_mask + decoded - subtract + 1481946885 & mask32
    value = operand - helper_mask & 16711935
    value ^= 15139057
    value = value - (value << 1 & 29491344) & mask32
    value = value + 3219216456 & mask32
    value ^= 1067679326
    value = value - (value << 1 & 3850504902) & mask32
    value = value + 4072736099 & mask32
    return value ^ 2145255346


def control__late_control_words(
    descriptor_address: int, helper_descriptor: int
) -> tuple[int, int]:
    """Fields +0x20/+0x40 for the observed static late-helper path."""
    core_mask = (
        descriptor_address ^ message__PROLOGUE_XOR
    ) * message__PROLOGUE_MULTIPLIER & 4294967295
    first = control__LATE_STATIC_COMPARE ^ core_mask
    solutions = [
        high << 16 | low
        for high in range(256)
        for low in range(256)
        if control__late_helper_state(high << 16 | low, helper_descriptor)
        == control__LATE_HELPER_STATE
    ]
    if solutions != [3276906]:
        raise AssertionError("late control candidate set changed")
    second = solutions[0] ^ core_mask
    return (first, second)


def control__transform(value: int) -> int:
    value ^= control__XOR1
    value = (
        value - (control__SUBTRACT_MASK & (value << 1 & control__MASK64))
        & control__MASK64
    )
    value = value + control__ADD2 & control__MASK64
    value ^= control__XOR3
    return value + control__ADD4 & control__MASK64


def control__invert_masked_subtraction(result: int) -> int:
    """Solve y=x-(mask&(x<<1)) bit-by-bit from low to high."""
    value = 0
    for bit in range(64):
        prefix_mask = (1 << bit + 1) - 1
        candidate = value | 1 << bit
        transformed = (
            candidate - (control__SUBTRACT_MASK & (candidate << 1 & control__MASK64))
            & prefix_mask
        )
        if transformed == result & prefix_mask:
            value = candidate
        elif (
            value - (control__SUBTRACT_MASK & (value << 1 & control__MASK64))
            & prefix_mask
            != result & prefix_mask
        ):
            raise AssertionError("masked subtraction inverse not unique")
    return value


def control__descriptor_link_word(node2_address: int) -> int:
    """Field +0x48 that makes node 1 point to chosen node 2."""
    decoded = node2_address - control__NODE_BIAS & control__MASK64
    value = decoded - control__ADD4 & control__MASK64
    value ^= control__XOR3
    value = value - control__ADD2 & control__MASK64
    value = control__invert_masked_subtraction(value)
    value ^= control__XOR1
    if control__transform(value) != decoded:
        raise AssertionError("control-link encoder failed round trip")
    return value
