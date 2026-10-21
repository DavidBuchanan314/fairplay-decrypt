#include "core4_seed.h"
#include "core4_modexp.h"
#include "core4_contexts.h"

#include <stddef.h>
#include <stdint.h>

static uint16_t fill_halfword(uint16_t context) {
    uint32_t word = (uint16_t)(context - UINT32_C(0x433a));
    uint32_t quotient = (uint32_t)(((uint64_t)word * UINT32_C(0x20005)) >> 32);
    uint32_t difference = word - quotient;
    quotient = (quotient + (difference >> 1)) >> 14;
    quotient -= quotient << 15;
    word += quotient;
    return (uint16_t)(((UINT32_C(0x7f4e) ^ word) + (UINT32_C(0xfe9c) & (word << 1))) - UINT32_C(0x4e00));
}

int core4_seed(uint16_t context, uint8_t output[38]) {
    if (!output) {
        return -1;
    }

    uint16_t filler = fill_halfword(context);
    uint16_t shifted = (uint16_t)(filler - UINT32_C(0x314e));
    uint32_t exponent = shifted + UINT32_C(0x12bb3b32) - (((uint32_t)shifted << 1) + 2 & UINT32_C(0x27662));
    uint16_t words[9];
    for (unsigned group = 0; group < 3; group++) {
        for (unsigned call = 0; call < 3; call++) {
            uint32_t temp = UINT32_C(0x4c17b7b2) + group + filler + UINT32_C(0x1701) + 3 * call;
            uint32_t encoded = (temp ^ UINT32_C(0x7675)) + (UINT32_C(0xecea) & (temp << 1)) - UINT32_C(0x6240);
            if (core4_modexp((uint16_t)encoded, exponent, &words[group * 3 + call])) {
                return -1;
            }
        }
    }

    output[0] = (uint8_t)filler;
    output[1] = (uint8_t)(filler >> 8);
    for (unsigned call = 0; call < 3; call++) {
        for (unsigned group = 0; group < 3; group++) {
            unsigned position = 2 + (call * 3 + group) * 2;
            uint16_t word = words[group * 3 + call];
            output[position] = (uint8_t)word;
            output[position + 1] = (uint8_t)(word >> 8);
        }
    }

    for (unsigned index = 0; index < 9; index++) {
        output[20 + 2 * index] = (uint8_t)filler;
        output[21 + 2 * index] = (uint8_t)(filler >> 8);
    }

    return 0;
}

int core4_seeds_from_digest(const uint8_t encoded[20], uint8_t first[38], uint8_t second[38]) {
    if (!encoded || !first || !second) {
        return -1;
    }

    uint16_t contexts[2];
    core4_contexts_from_digest(encoded, contexts);
    return core4_seed(contexts[0], first) || core4_seed(contexts[1], second) ? -1 : 0;
}
