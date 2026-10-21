#include "core4_native.h"
#include "core4_aes.h"
#include "core4_digest.h"
#include "core4_finish.h"
#include "core4_contexts.h"

#include <stddef.h>
#include <stdint.h>

static uint32_t word_at(const uint8_t table[CORE4_WORD_TABLE_SIZE], uint16_t context) {
    const uint8_t *value = table + (size_t)context * 4;
    return (uint32_t)value[0] | (uint32_t)value[1] << 8 | (uint32_t)value[2] << 16 | (uint32_t)value[3] << 24;
}

int core4_from_encoded_digest(const uint8_t encoded[20], const uint8_t word_table[CORE4_WORD_TABLE_SIZE], uint8_t output[16]) {
    if (!encoded || !word_table || !output) {
        return -1;
    }

    uint8_t first[16];
    if (core4_aes(encoded, first)) {
        return -1;
    }

    uint16_t contexts[2];
    core4_contexts_from_digest(encoded, contexts);
    uint8_t second[8];
    for (unsigned index = 0; index < 2; index++) {
        uint32_t word = word_at(word_table, contexts[index]);
        uint32_t mixed = word + UINT32_C(0x391cb4e8) - ((UINT32_C(0x80147e16) + 2 * word) & UINT32_C(0xf224ebba));
        second[4 * index + 0] = (uint8_t)((mixed >> 24) ^ 0x79);
        second[4 * index + 1] = (uint8_t)((mixed >> 16) ^ 0x12);
        second[4 * index + 2] = (uint8_t)((mixed >> 8) ^ 0x75);
        second[4 * index + 3] = (uint8_t)((mixed ^ UINT32_C(0xdddddddd)) & 0xff);
    }

    core4_finish(first, second, output);
    return 0;
}

int core4_context_native(const uint8_t message[16], const uint8_t word_table[CORE4_WORD_TABLE_SIZE], uint8_t output[16]) {
    if (!message || !word_table || !output) {
        return -1;
    }

    uint8_t encoded[20];
    core4_digest(message, encoded);
    return core4_from_encoded_digest(encoded, word_table, output);
}
