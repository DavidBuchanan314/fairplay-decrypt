#include "core4_finish.h"

#include "core4_tables.inc"

static uint8_t table_pair(uint8_t first, uint8_t second) {
    if (first == 0x78 || second == 0x5c) {
        return 0x59;
    }

    uint8_t left = INPUT_TABLE[(uint8_t)(first - 0x78)] ^ 0x19;
    uint8_t right = INPUT_TABLE[(uint8_t)(second - 0x5c)] ^ 0x19;
    return OUTPUT_TABLE[((unsigned)left + right) % 255];
}

static uint8_t first_byte(uint8_t value) {
    uint32_t mixed = (uint32_t)(value ^ 0x4a) + ((0xf4 & ((uint32_t)value << 1)) ^ 0x60) - 2;
    return (uint8_t)mixed;
}

static uint8_t second_byte(uint8_t value) {
    uint32_t mixed = (uint32_t)(value ^ 0xfe) + (((uint32_t)value << 1) & 0xfc) + 0x5e;
    return (uint8_t)mixed;
}

void core4_finish(const uint8_t first[8], const uint8_t second[8], uint8_t output[16]) {
    for (unsigned row = 0; row < 8; row++) {
        uint8_t result = 0x30;
        for (unsigned column = 0; column < 8; column++) {
            result ^= table_pair(first_byte(first[(row + column) & 7]), second_byte(second[column]));
        }

        output[row] = result;
        output[row + 8] = result;
    }
}
