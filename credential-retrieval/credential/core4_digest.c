#include "core4_digest.h"

#include <CommonCrypto/CommonDigest.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static const uint8_t PREFIX[17] = {0x5b, 0x53, 0xba, 0xe2, 0xa6, 0x6f, 0x01, 0x67, 0x73, 0x29, 0x10, 0xc5, 0x20, 0x99, 0xb7, 0x33, 0x81};
static const uint8_t SUFFIX[12] = {0x54, 0xda, 0x7b, 0x97, 0xe1, 0xf9, 0x0e, 0xc3, 0xae, 0x64, 0x6e, 0x3a};
static const uint8_t WORD_MASK[4] = {0x69, 0x5c, 0xfa, 0x3c};

void core4_digest(const uint8_t message[16], uint8_t encoded[20]) {
    uint8_t source[45];
    uint8_t digest[CC_SHA1_DIGEST_LENGTH];
    memcpy(source, PREFIX, sizeof(PREFIX));
    memcpy(source + sizeof(PREFIX), message, 16);
    memcpy(source + sizeof(PREFIX) + 16, SUFFIX, sizeof(SUFFIX));
    CC_SHA1(source, sizeof(source), digest);
    for (size_t word = 0; word < 5; word++) {
        uint8_t a = digest[word * 4 + 0] ^ WORD_MASK[0];
        uint8_t b = digest[word * 4 + 1] ^ WORD_MASK[1];
        uint8_t c = digest[word * 4 + 2] ^ WORD_MASK[2];
        uint8_t d = digest[word * 4 + 3] ^ WORD_MASK[3];
        uint8_t t = a ^ 0x69;
        encoded[word * 4 + 0] = (uint8_t)(t - 2 * ((t & 0x36) ^ (a & 6)) + 0x30);
        t = b ^ 0x5c;
        encoded[word * 4 + 1] = (uint8_t)(t - ((2 * t) & 0x60) + 0x30);
        t = c ^ 0xfa;
        encoded[word * 4 + 2] = (uint8_t)(t - ((2 * t) & 0x60) + 0x30);
        encoded[word * 4 + 3] = d ^ 0x0c;
    }
}
