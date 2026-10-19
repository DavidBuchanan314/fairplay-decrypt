"""Replay the obfuscated core-4 SHA-like routine.

Runs the bundled daemon's core-4 path using the raw license,
SIDB, device UDID, and independently obtained hardware side-write.
"""

import argparse
from pathlib import Path
from unicorn import Uc, UcError, UC_ARCH_ARM64, UC_MODE_ARM
from unicorn import (
    UC_HOOK_CODE,
    UC_HOOK_MEM_INVALID,
)
from unicorn import arm64_const
from offline import (
    macho__segments as segments,
    rebases__rebase_addresses as rebase_addresses,
    message__fields as derive_message_fields,
    control__descriptor_link_word as descriptor_link_word,
    control__initial_branch_word as initial_branch_word,
    control__late_control_words as late_control_words,
    core3__derive as derive_core3,
    atoms__descendants as descendants,
    atoms__users as parse_users,
    atoms__saved_descriptor_key as saved_descriptor_key,
    runtime__udid as device_udid,
    priv_transform__atom as sinf_atom,
    keybag_layers__derive_keybag as derive_keybag,
)

ROOT = Path(__file__).resolve().parent
IMAGE = ROOT / "fairplayd.A2.ios14"
PAGE = 4096
IMAGE_BASE = 4294967296
ENTRY_OFFSET = 3119956


def page_down(value: int) -> int:
    return value & ~(PAGE - 1)


def page_up(value: int) -> int:
    return value + PAGE - 1 & ~(PAGE - 1)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--license-sinf", type=Path, required=True)
    parser.add_argument("--hardware-sidewrite-hex", required=True)
    args = parser.parse_args()
    runtime_base = IMAGE_BASE
    synthetic_stack_base = 412316860416
    regs = {f"x{index}": "0x0" for index in range(29)}
    regs.update(
        {
            "x0": hex(synthetic_stack_base + 68848),
            "x30": hex(481036337152),
            "sp": hex(synthetic_stack_base + 8192),
            "pc": hex(runtime_base + ENTRY_OFFSET),
        }
    )
    expected_pc = runtime_base + ENTRY_OFFSET
    if int(regs["pc"], 16) != expected_pc:
        raise AssertionError("entry PC is not the expected daemon function")
    image = IMAGE.read_bytes()
    allocator_stub = 8589934592
    free_stub = 8858370048
    guard_addr = 13153337344
    engine = Uc(UC_ARCH_ARM64, UC_MODE_ARM)
    mapped: set[int] = set()

    def map_region(base: int, data: bytes, allocated: int | None = None) -> None:
        size = len(data) if allocated is None else allocated
        pending = [
            page
            for page in range(page_down(base), page_up(base + size), PAGE)
            if page not in mapped
        ]
        if pending:
            run_start = run_end = pending[0]
            for page in pending[1:] + [None]:
                if page is not None and page == run_end + PAGE:
                    run_end = page
                else:
                    engine.mem_map(run_start, run_end + PAGE - run_start)
                    if page is not None:
                        run_start = run_end = page
            mapped.update(pending)
        if data:
            engine.mem_write(base, data)

    for name, address, vmsize, fileoff, filesize in segments(image):
        if name == "__PAGEZERO" or vmsize == 0:
            continue
        base = runtime_base + address - IMAGE_BASE
        if name == "__DATA_CONST":
            data = bytearray(image[fileoff : fileoff + filesize])
            slide = runtime_base - IMAGE_BASE
            for pointer in rebase_addresses(image):
                if address <= pointer < address + vmsize:
                    offset = pointer - address
                    raw = int.from_bytes(data[offset : offset + 8], "little")
                    data[offset : offset + 8] = (
                        raw + slide & 18446744073709551615
                    ).to_bytes(8, "little")
            data[16:24] = guard_addr.to_bytes(8, "little")
            raw_malloc_bias = int.from_bytes(data[294744:294752], "little")
            data[294744:294752] = (allocator_stub + raw_malloc_bias).to_bytes(
                8, "little"
            )
            raw_free_bias = int.from_bytes(data[290640:290648], "little")
            data[290640:290648] = (free_stub + raw_free_bias).to_bytes(8, "little")
            for offset in (274632, 291664, 303832, 312648):
                data[offset : offset + 8] = bytes(8)
            data = bytes(data)
            if len(data) != vmsize:
                raise AssertionError("DATA_CONST length differs from segment")
        else:
            data = image[fileoff : fileoff + filesize]
        map_region(base, data, vmsize)
    keybag_output_base = None
    for region in ("stack", "guid-record"):
        if region == "stack":
            base = synthetic_stack_base
            data = bytes(73728)
        elif region == "guid-record":
            udid = device_udid()
            if len(udid) != 20:
                raise AssertionError("saved UDID must be 20 bytes")
            data = len(udid).to_bytes(4, "little") + udid + bytes(256 - 24)
            base = 13421772800
        else:
            raise AssertionError(f"synthetic {region} setup unavailable")
        if region == "stack":
            map_region(base - PAGE, bytes(PAGE))
        map_region(base, data)
        if region == "stack":
            engine.mem_write(
                int(regs["x0"], 16) + 24, (13421772800).to_bytes(8, "little")
            )
            udid = device_udid()
            sinf = args.license_sinf.read_bytes()
            descriptor = int(regs["x0"], 16)
            calculated = derive_message_fields(udid, sinf, descriptor)
            for delta, name in (
                (0, "dispatch_word"),
                (8, "field8_low32"),
                (40, "field40_low32"),
            ):
                engine.mem_write(
                    descriptor + delta, calculated[name].to_bytes(4, "little")
                )
            engine.mem_write(descriptor + 12, bytes(4))
            engine.mem_write(descriptor + 44, bytes(4))
            descriptor = int(regs["x0"], 16)
            node2 = int(regs["sp"], 16) - 3664 + 6984
            node3 = descriptor + 216
            helper_descriptor = int(regs["sp"], 16) - 3664 + 3504
            late_first, late_second = late_control_words(descriptor, helper_descriptor)
            engine.mem_write(descriptor + 32, late_first.to_bytes(4, "little"))
            engine.mem_write(descriptor + 64, late_second.to_bytes(4, "little"))
            engine.mem_write(
                descriptor + 48,
                initial_branch_word(descriptor, image, runtime_base).to_bytes(
                    4, "little"
                ),
            )
            engine.mem_write(
                descriptor + 72, descriptor_link_word(node2).to_bytes(8, "little")
            )
            engine.mem_write(node2, bytes(4))
            engine.mem_write(node2 + 8, (node2 + node3).to_bytes(8, "little"))
            engine.mem_write(node3, bytes(4))
            engine.mem_write(node3 + 8, node3.to_bytes(8, "little"))
            sidewrite = bytes.fromhex(args.hardware_sidewrite_hex)
            if len(sidewrite) != 16 or not args.license_sinf:
                raise AssertionError("side-write must be 16 bytes with SINF input")
            sinf = args.license_sinf.read_bytes()
            core3 = derive_core3(sinf, sidewrite)
            clear = derive_keybag(saved_descriptor_key())[-1]
            users = parse_users(clear)
            account = sinf_atom(sinf, b"user")
            selected = [user for user in users if user.meta[:4] == account]
            if len(selected) != 1:
                raise AssertionError("selected user count is not one")
            scra = list(descendants(selected, "scra"))
            if len(scra) != 1 or len(scra[0].payload) != 49:
                raise AssertionError("selected scra is malformed")
            keybag_output_base = 13690208256
            map_region(keybag_output_base, bytes(PAGE))
            engine.mem_write(keybag_output_base, core3 + scra[0].payload)
            engine.mem_write(
                int(regs["x0"], 16) + 56, keybag_output_base.to_bytes(8, "little")
            )
    for index in range(29):
        engine.reg_write(
            getattr(arm64_const, f"UC_ARM64_REG_X{index}"), int(regs[f"x{index}"], 16)
        )
    engine.reg_write(arm64_const.UC_ARM64_REG_X29, 0)
    engine.reg_write(arm64_const.UC_ARM64_REG_X30, int(regs["x30"], 16))
    engine.reg_write(arm64_const.UC_ARM64_REG_SP, int(regs["sp"], 16))
    return_address = int(regs["x30"], 16)
    engine.mem_map(page_down(return_address), PAGE)
    engine.mem_write(return_address, bytes.fromhex("1f2003d5"))
    executed = 0
    stopped = False
    recent: list[int] = []
    allocated = 0
    freed = 0
    heap = 12884901888
    engine.mem_map(heap, PAGE)
    engine.mem_map(guard_addr, PAGE)

    def on_code(uc: Uc, address: int, size: int, _) -> None:
        nonlocal executed, stopped
        executed += 1
        recent.append(address)
        if len(recent) > 12:
            recent.pop(0)
        if address == return_address:
            stopped = True
            print(f"returned after {executed} instructions")
            uc.emu_stop()
            return

    def on_invalid(uc: Uc, access: int, address: int, size: int, value: int, _) -> bool:
        nonlocal allocated, freed
        pc = uc.reg_read(arm64_const.UC_ARM64_REG_PC)
        if address == guard_addr and access == 19:
            uc.mem_map(page_down(address), PAGE)
            return True
        if address == allocator_stub and access == 21:
            requested = uc.reg_read(arm64_const.UC_ARM64_REG_X0)
            if requested != 56 or allocated:
                raise AssertionError("unexpected first allocator call")
            uc.mem_map(page_down(address), PAGE)
            uc.mem_write(address, bytes.fromhex("c0035fd6"))
            uc.reg_write(arm64_const.UC_ARM64_REG_X0, heap)
            allocated += 1
            print(f"modeled 56-byte allocation at {heap:#x}")
            return True
        if address == free_stub and access == 21:
            if uc.reg_read(arm64_const.UC_ARM64_REG_X0) != heap or freed:
                raise AssertionError("unexpected free target or duplicate free")
            uc.mem_map(page_down(address), PAGE)
            uc.mem_write(address, bytes.fromhex("c0035fd6"))
            freed += 1
            print("modeled free of 56-byte allocation")
            return True
        print(
            f"unmapped access={access} address={address:#x} size={size} pc={pc:#x} daemon_offset={pc - runtime_base:#x} recent={[hex(value - runtime_base) for value in recent]}"
        )
        return False

    engine.hook_add(UC_HOOK_CODE, on_code)
    engine.hook_add(UC_HOOK_MEM_INVALID, on_invalid)
    try:
        engine.emu_start(expected_pc, 0, count=500000)
    except UcError as error:
        pc = engine.reg_read(arm64_const.UC_ARM64_REG_PC)
        print(
            f"emulation stopped: {error}; pc={pc:#x} daemon_offset={pc - runtime_base:#x}; executed={executed}"
        )
    if not stopped:
        raise RuntimeError(
            f"core-4 did not return: pc={engine.reg_read(arm64_const.UC_ARM64_REG_PC):#x}, steps={executed}"
        )
    if keybag_output_base is not None:
        print(
            "keybag-output-first16="
            + bytes(engine.mem_read(keybag_output_base, 16)).hex()
        )


if __name__ == "__main__":
    main()
