#include "core4_modexp.h"

#include <stdint.h>

int core4_modexp(uint16_t base, uint32_t encoded_exponent, uint16_t *result) {
    if (!result) {
        return -1;
    }

    if (encoded_exponent == UINT32_C(0x12bb3b30)) {
        uint32_t source = (((uint32_t)(base < 0x1435) << 16) | base) - UINT32_C(0x1435);
        uint32_t value = source % UINT32_C(0x7fff);
        *result = (uint16_t)((value ^ UINT32_C(0x5d6b)) + ((value << 1) & UINT32_C(0xbad6)) - UINT32_C(0x4022));
        return 0;
    }

    uint32_t state = (((uint32_t)(base < 0x1435) << 16) | base) + UINT32_C(0x2f349dbf);
    uint32_t accumulator = UINT32_C(0x4995d8f9);
    uint32_t square = 0;
    for (unsigned iteration = 0; iteration < 64; iteration++) {
        uint32_t next = accumulator;
        if (!(encoded_exponent & 1)) {
            uint32_t product = (accumulator ^ UINT32_C(0x4995d8f8)) * (state - UINT32_C(0x2f34b1f4));
            uint32_t reduced = product % UINT32_C(0x7fff);
            next = reduced + UINT32_C(0x4995d8f8) - ((reduced << 1) & UINT32_C(0xb1f0));
        }

        uint32_t source = state - UINT32_C(0x2f34b1f4);
        uint32_t reduced = (source * source) % UINT32_C(0x7fff);
        square = (reduced ^ UINT32_C(0xaf7db3f4)) + ((reduced << 1) & UINT32_C(0x67e8));
        state = square + UINT32_C(0x7fb6fe00);
        encoded_exponent = (encoded_exponent >> 1) ^ UINT32_C(0x1be6a6a9);
        accumulator = next;
        if (encoded_exponent == UINT32_C(0x12bb3b30)) {
            uint32_t product = (square + UINT32_C(0x50824c0c)) * (accumulator ^ UINT32_C(0x4995d8f8));
            uint32_t value = product % UINT32_C(0x7fff);
            *result = (uint16_t)((value ^ UINT32_C(0x5d6b)) + ((value << 1) & UINT32_C(0xbad6)) - UINT32_C(0x4022));
            return 0;
        }
    }

    return -1;
}
