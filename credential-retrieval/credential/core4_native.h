#ifndef CORE4_NATIVE_H
#define CORE4_NATIVE_H

#include <stdint.h>

#define CORE4_WORD_TABLE_SIZE (65536u * 4u)

int core4_context_native(const uint8_t message[16], const uint8_t word_table[CORE4_WORD_TABLE_SIZE], uint8_t output[16]);
int core4_from_encoded_digest(const uint8_t encoded[20], const uint8_t word_table[CORE4_WORD_TABLE_SIZE], uint8_t output[16]);

#endif
