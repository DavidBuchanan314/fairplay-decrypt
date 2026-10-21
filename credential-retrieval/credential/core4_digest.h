#ifndef CORE4_DIGEST_H
#define CORE4_DIGEST_H

#include <stdint.h>

void core4_digest(const uint8_t message[16], uint8_t encoded[20]);

#endif
