"""Build the device's selector request from the license and bundled daemon.

The earlier daemon function supplies its nonce-pair mask; the later function
produces the 552-byte request used by the standalone on-device helper.
"""

import argparse
from pathlib import Path
import subprocess
import sys
from unicorn import (
    Uc,
    UcError,
    UC_ARCH_ARM64,
    UC_MODE_ARM,
    UC_HOOK_CODE,
    UC_HOOK_MEM_INVALID,
    UC_HOOK_MEM_WRITE,
)
from unicorn import arm64_const
from offline import (
    macho__segments as segments,
    body__derive_body as derive_body,
    encoder_seed__derive_encoder_seed as derive_encoder_seed,
)

ROOT = Path(__file__).resolve().parent
DAEMON = ROOT / "fairplayd.A2.ios14"
PAGE = 4096
IMAGE_BASE = 4294967296
SUPPORTED_ENTRY_OFFSETS = {10610772, 10612920}
STOP_OFFSET = 10612760
INITIAL_STACK_RANGES_MIDDLE = (
    (4096, 4120),
    (4336, 4344),
    (4480, 4496),
    (4656, 4664),
    (4672, 4768),
    (7016, 7024),
    (8076, 8080),
    (10444, 10448),
    (13336, 13344),
    (18092, 18096),
    (18256, 18264),
    (19380, 19384),
    (20264, 20272),
    (26552, 26556),
    (28688, 28696),
    (31576, 31584),
    (32072, 32080),
    (48860, 48861),
    (48864, 49024),
    (49392, 49424),
)
INITIAL_STACK_RANGES_EARLY = (
    (6344, 6352),
    (7404, 7408),
    (9772, 9776),
    (12664, 12672),
    (17420, 17424),
    (17584, 17592),
    (18708, 18712),
    (19592, 19600),
    (25880, 25884),
    (28016, 28024),
    (30904, 30912),
    (31400, 31408),
    (48188, 48189),
    (48192, 48352),
    (48720, 48752),
    (64752, 64772),
    (64776, 64796),
)
INITIAL_STACK_RANGES = INITIAL_STACK_RANGES_MIDDLE


def compute_early_fields() -> tuple[bytes, bytes]:
    """Execute the original earlier daemon function to generate two frame fields."""
    run = subprocess.run(
        [
            sys.executable,
            str(Path(__file__).resolve()),
            "--fully-synthetic-early-mask",
            "--show-stack-range",
            "0xd60:0xd70",
            "--show-stack-range",
            "0xee0:0xef0",
            "--instruction-limit",
            "30000",
        ],
        check=True,
        capture_output=True,
        text=True,
        timeout=90,
    )
    if "outcome=wrote stack +0xee0..+0xef0" not in run.stdout:
        raise AssertionError("synthetic early daemon replay did not produce the mask")
    fields = []
    for lo, hi in ((3424, 3440), (3808, 3824)):
        prefix = f"final-stack +{lo:#x}..+{hi:#x}: "
        values = [
            line[len(prefix) :]
            for line in run.stdout.splitlines()
            if line.startswith(prefix)
        ]
        if len(values) != 1:
            raise AssertionError(f"early daemon field {lo:#x} missing")
        field = bytes.fromhex(values[0])
        if len(field) != 16:
            raise AssertionError(f"early daemon field {lo:#x} length changed")
        fields.append(field)
    return (fields[0], fields[1])


def synthetic_middle_state(
    image_base: int,
    stack_base: int,
    sinf: bytes,
    body_address: int,
    nonce: bytes,
    nonce_pair_mask: bytes,
    control_frame: bytes,
) -> tuple[dict[str, str], bytes]:
    """Build the later entry without consulting a live-process snapshot."""
    if len(nonce) != 16:
        raise ValueError("synthetic nonce must be 16 bytes")
    if len(nonce_pair_mask) != 16:
        raise ValueError("synthetic nonce-pair mask must be 16 bytes")
    if len(control_frame) != 16:
        raise ValueError("synthetic control frame must be 16 bytes")
    stack = bytearray(77824)

    def write(offset: int, value: bytes) -> None:
        stack[offset : offset + len(value)] = value

    def word(offset: int, value: int) -> None:
        write(offset, value.to_bytes(8, "little"))

    write(4096, control_frame)
    word(4112, stack_base + 65424)
    word(4336, stack_base + 48864)
    write(4480, bytes((a ^ b for (a, b) in zip(nonce, nonce_pair_mask))))
    words = (
        18446744073671523972,
        stack_base + 31124,
        image_base + 16329552,
        3413736857,
        stack_base + 65032,
        2127255500,
        image_base + 14996432,
        image_base + 12799488,
        0,
        21678,
        stack_base + 65616,
        image_base + 1674932,
    )
    for index, value in enumerate(words):
        word(4672 + index * 8, value)
    word(7016, image_base + 2721024)
    write(10444, (431964479).to_bytes(4, "little"))
    word(13336, stack_base + 48864)
    write(18092, (1089178628).to_bytes(4, "little"))
    word(18256, stack_base + 16652)
    write(19380, (2127255660).to_bytes(4, "little"))
    word(20264, image_base + 1675188)
    word(28688, stack_base + 14616)
    word(31576, stack_base + 49408)
    word(32072, stack_base + 48816)
    write(48860, b"\x01")
    write(48864, derive_body(sinf, body_address))
    write(49392, derive_encoder_seed(DAEMON))
    write(49408, nonce)
    values = {
        "pc": image_base + 10612920,
        "sp": stack_base + 4096,
        "x0": stack_base + 65424,
        "x1": 740584316,
        "x2": stack_base + 4480,
        "x3": image_base + 10611888,
        "x4": image_base + 10612008,
        "x5": 1,
        "x6": 14,
        "x7": 218,
        "x8": 4294924397,
        "x9": stack_base + 49392,
        "x10": 10,
        "x11": stack_base + 4480,
        "x12": stack_base + 49403 - 16754942541229025445 & 18446744073709551615,
        "x13": 15,
        "x14": image_base + 10612920,
        "x15": 15,
        "x16": 0,
        "x17": 3741309067,
        "x18": 0,
        "x19": 185,
        "x20": 0,
        "x21": image_base + 12799488,
        "x22": image_base + 15536672,
        "x23": image_base + 16329552,
        "x24": stack_base + 65032,
        "x25": 3413736857,
        "x26": image_base + 16329552,
        "x27": stack_base + 31124,
        "x28": 18446744073671523972,
        "x29": stack_base + 4752,
        "x30": image_base + 1674932,
    }
    return ({name: hex(value) for (name, value) in values.items()}, bytes(stack))


def synthetic_early_mask_state() -> tuple[int, int, dict[str, str], bytes]:
    """Build an early encoder entry for the nonce-pair-mask producer only.

    The chosen virtual bases and some control-frame words reproduce one
    observed valid ABI state; no saved process file is read. The randomized
    160-byte body, nonce, fixed16 seed, external lookup word, and a variable
    stack word are deliberately zero. This is not a full early encoder input.
    """
    image_base = 4363714560
    stack_base = 6103095856
    stack = bytearray(77824)

    def write(offset: int, value: bytes) -> None:
        stack[offset : offset + len(value)] = value

    def word(offset: int, value: int) -> None:
        write(offset, value.to_bytes(8, "little"))

    word(6344, image_base + 2721024)
    write(9772, (431964479).to_bytes(4, "little"))
    word(12664, stack_base + 48192)
    write(17420, (1089178628).to_bytes(4, "little"))
    word(17584, stack_base + 15980)
    write(18708, (2127255660).to_bytes(4, "little"))
    word(19592, image_base + 1675188)
    word(28016, stack_base + 13944)
    word(30904, stack_base + 48736)
    word(31400, stack_base + 48144)
    write(48188, b"\x01")
    word(64752, stack_base + 48192)
    write(64760, bytes.fromhex("e62fbd29897c7a17109ee410"))
    word(64776, stack_base + 48736)
    write(64784, bytes.fromhex("8d02e0051d9254586ccd3f1b"))
    values = {
        "pc": image_base + 10610772,
        "sp": stack_base + 4096,
        "x0": stack_base + 64752,
        "x1": 0,
        "x2": 0,
        "x3": 0,
        "x4": 0,
        "x5": 0,
        "x6": 0,
        "x7": image_base + 16452448,
        "x8": image_base + 10610772,
        "x9": 18446744071954022522,
        "x10": stack_base + 48736,
        "x11": stack_base + 48192,
        "x12": 0,
        "x13": 2908295952,
        "x14": 0,
        "x15": 0,
        "x16": 0,
        "x17": image_base + 1674648,
        "x18": 0,
        "x19": 0,
        "x20": 0,
        "x21": image_base + 12799488,
        "x22": image_base + 14996432,
        "x23": 0,
        "x24": stack_base + 64360,
        "x25": 3413736857,
        "x26": image_base + 16329552,
        "x27": stack_base + 30452,
        "x28": 18446744073671523972,
        "x29": stack_base + 64944,
        "x30": image_base + 1674932,
    }
    return (
        image_base,
        stack_base,
        {name: hex(value) for (name, value) in values.items()},
        bytes(stack),
    )


def page_down(value: int) -> int:
    return value & ~(PAGE - 1)


def page_up(value: int) -> int:
    return value + PAGE - 1 & ~(PAGE - 1)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--fully-synthetic-middle", action="store_true")
    mode.add_argument("--fully-synthetic-early-mask", action="store_true")
    parser.add_argument("--license-sinf", type=Path)
    parser.add_argument("--output-request", type=Path)
    parser.add_argument(
        "--synthetic-selector-body-address", type=lambda value: int(value, 0)
    )
    parser.add_argument("--synthetic-nonce", default="11" * 16)
    parser.add_argument("--instruction-limit", type=int, default=300000)
    parser.add_argument(
        "--stop-offset", type=lambda value: int(value, 0), default=STOP_OFFSET
    )
    parser.add_argument("--show-stack-range", action="append", default=[])
    args = parser.parse_args()
    args.external_table = None
    args.external_table_base = 5367660544
    args.stop_after_stack_write_range = None
    if args.fully_synthetic_middle and args.fully_synthetic_early_mask:
        parser.error("choose one synthetic encoder boundary")
    if args.fully_synthetic_early_mask and args.stop_after_stack_write_range is None:
        args.stop_after_stack_write_range = "0xee0:0xef0"
    stop_write_range = None
    if args.stop_after_stack_write_range:
        lo_text, sep, hi_text = args.stop_after_stack_write_range.partition(":")
        if not sep:
            parser.error("--stop-after-stack-write-range requires START:STOP")
        lo, hi = (int(lo_text, 0), int(hi_text, 0))
        if not 0 <= lo < hi <= 73728 or hi - lo > 256:
            parser.error("stop-after-write range must be inside stack and <=256 bytes")
        stop_write_range = (lo, hi)
    if args.synthetic_selector_body_address is not None and (not args.license_sinf):
        parser.error("--synthetic-selector-body-address needs a file-derived SINF body")
    if args.fully_synthetic_middle:
        if not args.license_sinf:
            parser.error("synthetic middle needs the raw license record")
        if args.synthetic_selector_body_address is None:
            parser.error("synthetic middle needs --synthetic-selector-body-address")
        runtime_base = IMAGE_BASE
        stack_base = 412316860416
        sinf = args.license_sinf.read_bytes()
        control_frame, nonce_pair_mask = compute_early_fields()
        registers, synthetic_stack = synthetic_middle_state(
            runtime_base,
            stack_base,
            sinf,
            args.synthetic_selector_body_address,
            bytes.fromhex(args.synthetic_nonce),
            nonce_pair_mask,
            control_frame,
        )
        events = [
            {
                "kind": "entry-registers",
                "imageBase": hex(runtime_base),
                "entry": registers["pc"],
                "registers": registers,
            },
            {
                "kind": "memory",
                "name": "stack",
                "base": hex(stack_base),
                "length": len(synthetic_stack),
            },
        ]
    elif args.fully_synthetic_early_mask:
        if args.license_sinf is not None:
            parser.error("synthetic early mask takes no license input")
        runtime_base, stack_base, registers, synthetic_stack = (
            synthetic_early_mask_state()
        )
        events = [
            {
                "kind": "entry-registers",
                "imageBase": hex(runtime_base),
                "entry": registers["pc"],
                "registers": registers,
            },
            {
                "kind": "memory",
                "name": "stack",
                "base": hex(stack_base),
                "length": len(synthetic_stack),
            },
            {
                "kind": "memory",
                "name": "request",
                "base": hex(stack_base + 48160),
                "length": 552,
            },
        ]
    else:
        parser.error("choose a synthetic encoder mode")
    entry = next((item for item in events if item.get("kind") == "entry-registers"))
    stack_base = int(
        next(
            (
                item["base"]
                for item in events
                if item.get("kind") == "memory" and item.get("name") == "stack"
            )
        ),
        16,
    )
    registers = entry["registers"]
    runtime_base = int(entry["imageBase"], 16)
    entry_offset = int(entry["entry"], 16) - runtime_base
    if entry_offset not in SUPPORTED_ENTRY_OFFSETS:
        raise AssertionError("unsupported daemon encoder boundary")
    image = DAEMON.read_bytes()
    engine = Uc(UC_ARCH_ARM64, UC_MODE_ARM)
    mapped = set()

    def map_region(base: int, data: bytes, allocated: int | None = None) -> None:
        start = page_down(base)
        end = page_up(base + (allocated if allocated is not None else len(data)))
        for page in range(start, end, PAGE):
            if page not in mapped:
                engine.mem_map(page, PAGE)
                mapped.add(page)
        if data:
            engine.mem_write(base, data)

    for name, address, vmsize, fileoff, filesize in segments(image):
        if name == "__PAGEZERO" or vmsize == 0:
            continue
        base = runtime_base + address - IMAGE_BASE
        if name == "__DATA_CONST":
            synthesized = bytearray(image[fileoff : fileoff + filesize])
            if len(synthesized) != vmsize:
                raise AssertionError("DATA_CONST file size differs from VM size")
            slide = runtime_base - IMAGE_BASE
            for offset in range(64, len(synthesized), 8):
                raw = int.from_bytes(synthesized[offset : offset + 8], "little")
                if raw:
                    synthesized[offset : offset + 8] = (raw + slide).to_bytes(
                        8, "little"
                    )
            synthesized[16:24] = (1879048192).to_bytes(8, "little")
            data = bytes(synthesized)
        elif name == "__DATA":
            synthesized = bytearray(vmsize)
            synthesized[:filesize] = image[fileoff : fileoff + filesize]
            if entry_offset == 10610772:
                if args.external_table is None and (
                    not args.fully_synthetic_early_mask
                ):
                    parser.error("early synthetic globals need --external-table")
                synthesized[68448:68452] = (255).to_bytes(4, "little")
                synthesized[72552:72560] = (args.external_table_base + 89448).to_bytes(
                    8, "little"
                )
            data = bytes(synthesized)
        else:
            data = image[fileoff : fileoff + filesize]
        map_region(base, data, vmsize)
    for item in events:
        if item.get("kind") != "memory" or item.get("name") in (
            "data_const",
            "data",
            "request",
        ):
            continue
        if item["name"].startswith("external-"):
            continue
        if item["name"] != "stack":
            raise AssertionError("unexpected synthetic memory region")
        data = synthetic_stack
        if len(data) != item["length"]:
            raise AssertionError(f"{item['name']} length changed")
        base = int(item["base"], 16)
        map_region(base, data)
    if args.fully_synthetic_early_mask:
        table = bytearray(90112)
        map_region(args.external_table_base, bytes(table))
    for index in range(31):
        text = registers.get(f"x{index}")
        if text is None or text == "undefined":
            raise AssertionError(f"missing x{index} at entry")
        engine.reg_write(getattr(arm64_const, f"UC_ARM64_REG_X{index}"), int(text, 16))
    engine.reg_write(arm64_const.UC_ARM64_REG_SP, int(registers["sp"], 16))
    engine.reg_write(arm64_const.UC_ARM64_REG_PC, int(registers["pc"], 16))
    request_item = next(
        (item for item in events if item.get("name") == "request"), None
    )
    if request_item is not None:
        request_body = int(request_item["base"], 16) + 32
    else:
        if entry_offset != 10612920:
            raise AssertionError(
                "request address unavailable for this encoder boundary"
            )
        request_body = int(registers["x9"], 16) - 528
    print("entry-request-body-prefix:", bytes(engine.mem_read(request_body, 16)).hex())
    reached = set()
    recent_offsets: list[int] = []
    result = {"steps": 0, "outcome": "instruction limit"}
    milestones = {10623720, 10634168, 10636300, STOP_OFFSET, args.stop_offset}
    target_writes: set[int] = set()

    def on_write(
        _uc: Uc, _access: int, address: int, size: int, _value: int, _user: object
    ) -> None:
        if stop_write_range is not None:
            lo, hi = stop_write_range
            for byte in range(
                max(address, stack_base + lo), min(address + size, stack_base + hi)
            ):
                target_writes.add(byte - stack_base)
            if len(target_writes) == hi - lo:
                result["pending-stop-after-write"] = True

    def on_code(uc: Uc, address: int, size: int, _: object) -> None:
        result["steps"] += 1
        if result.pop("pending-stop-after-write", False):
            lo, hi = stop_write_range
            result["outcome"] = f"wrote stack +{lo:#x}..+{hi:#x}"
            uc.emu_stop()
            return
        offset = address - runtime_base
        recent_offsets.append(offset)
        if len(recent_offsets) > 12:
            recent_offsets.pop(0)
        if offset in milestones and offset not in reached:
            reached.add(offset)
            print(f"reached daemon +{offset:#x} at step {result['steps']}")
        if offset == args.stop_offset:
            result["outcome"] = f"reached daemon +{offset:#x}"
            uc.emu_stop()

    def on_invalid(
        uc: Uc, access: int, address: int, size: int, value: int, _: object
    ) -> bool:
        pc = uc.reg_read(arm64_const.UC_ARM64_REG_PC)
        if pc - runtime_base in (10610812, 10612876) and size == 8:
            page = page_down(address)
            if page not in mapped:
                uc.mem_map(page, PAGE)
                mapped.add(page)
            canary = (
                1311768467463790320
                if pc - runtime_base == 10610812
                else uc.reg_read(arm64_const.UC_ARM64_REG_X8)
            )
            uc.mem_write(address, canary.to_bytes(8, "little"))
            print(f"modeled external stack guard at {address:#x}")
            return True
        result["outcome"] = (
            f"unmapped access kind={access} addr={address:#x} size={size} pc={pc:#x} daemon-offset={pc - runtime_base:#x}"
        )
        print("recent-daemon-offsets:", [f"{offset:#x}" for offset in recent_offsets])
        print(
            "at-fault-registers:",
            {
                name: hex(
                    uc.reg_read(getattr(arm64_const, f"UC_ARM64_REG_{name.upper()}"))
                )
                for name in (
                    "x7",
                    "x8",
                    "x9",
                    "x10",
                    "x11",
                    "x16",
                    "x17",
                    "x19",
                    "x21",
                    "x24",
                    "x30",
                )
            },
        )
        return False

    engine.hook_add(UC_HOOK_CODE, on_code)
    engine.hook_add(UC_HOOK_MEM_INVALID, on_invalid)
    if stop_write_range is not None:
        engine.hook_add(UC_HOOK_MEM_WRITE, on_write)
    try:
        engine.emu_start(runtime_base + entry_offset, 0, count=args.instruction_limit)
    except UcError as error:
        if result["outcome"] == "instruction limit":
            result["outcome"] = f"Unicorn error: {error}"
    print(f"steps={result['steps']} outcome={result['outcome']}")
    for spec in args.show_stack_range:
        lo_text, sep, hi_text = spec.partition(":")
        if not sep:
            parser.error("--show-stack-range requires START:STOP")
        lo, hi = (int(lo_text, 0), int(hi_text, 0))
        saved_stack = next((item for item in events if item.get("name") == "stack"))
        if not 0 <= lo < hi <= saved_stack["length"] or hi - lo > 256:
            parser.error("--show-stack-range must fit stack and span <=256 bytes")
        print(
            f"final-stack +{lo:#x}..+{hi:#x}: {bytes(engine.mem_read(stack_base + lo, hi - lo)).hex()}"
        )
    last = bytes(engine.mem_read(request_body + 9 * 16, 16))
    print(f"request-body={request_body:#x} last-block={last.hex()}")
    if args.output_request is not None:
        if result["outcome"] != "reached daemon +0x199150":
            raise AssertionError("request output requires --stop-offset 0x199150")
        if args.output_request.exists():
            raise FileExistsError(args.output_request)
        args.output_request.write_bytes(bytes(engine.mem_read(request_body - 32, 552)))
        print("wrote selector request:", args.output_request)


if __name__ == "__main__":
    main()
