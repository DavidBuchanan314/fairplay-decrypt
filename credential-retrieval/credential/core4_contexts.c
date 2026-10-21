#include "core4_contexts.h"

#include <stdint.h>

static uint16_t context_halfword(uint8_t a, uint8_t b, uint32_t xor_constant, uint32_t and_constant, uint32_t subtract_constant) {
    uint32_t first = a ^ UINT32_C(0x30);
    first -= UINT32_C(0x1a) & (first << 1);
    first = (first << 8) + UINT32_C(0xd00);
    uint32_t temp = first ^ UINT32_C(0xd00);
    uint32_t middle = first & UINT32_C(0x4000);
    uint32_t word = temp - 2 * (middle ^ temp) + UINT32_C(0x3fe9);
    uint32_t second = b ^ UINT32_C(0x30);
    second -= UINT32_C(0x86) & (second << 1);
    second += UINT32_C(0x1343);
    word = (word ^ UINT32_C(0x3fe9)) | (second ^ UINT32_C(0x1343));
    word -= UINT32_C(0xffff9316) & (word << 1);
    word += UINT32_C(0x498b);
    return (uint16_t)((word ^ xor_constant) + ((and_constant & (word << 1)) ^ UINT32_C(0x9214)) - subtract_constant);
}

void core4_contexts_from_digest(const uint8_t encoded[20], uint16_t contexts[2]) {
    contexts[0] = context_halfword(encoded[16], encoded[17], UINT32_C(0x02f1), UINT32_C(0x96f4), UINT32_C(0x0840));
    contexts[1] = context_halfword(encoded[18], encoded[19], UINT32_C(0x32b1), UINT32_C(0xf674), UINT32_C(0x3800));
}
