#include "core4_aes.h"

#include <CommonCrypto/CommonCryptor.h>
#include <stddef.h>
#include <stdint.h>

static const uint8_t KEY[16] = {0x42, 0xb3, 0x16, 0x0c, 0x77, 0xa8, 0xaf, 0x81, 0x90, 0x55, 0x96, 0xde, 0x35, 0x3e, 0x0f, 0x07};

int core4_aes(const uint8_t input[16], uint8_t output[16]) {
    uint8_t plain[16];
    uint8_t encrypted[16];
    for (size_t index = 0; index < 16; index++) {
        plain[index] = input[index] ^ 0x30;
    }

    size_t written = 0;
    CCCryptorStatus status = CCCrypt(kCCEncrypt, kCCAlgorithmAES, kCCOptionECBMode, KEY, sizeof(KEY), NULL, plain, sizeof(plain), encrypted, sizeof(encrypted), &written);
    if (status != kCCSuccess || written != sizeof(encrypted)) {
        return -1;
    }

    for (size_t index = 0; index < 16; index++) {
        output[index] = encrypted[index] ^ 0x30;
    }

    return 0;
}
