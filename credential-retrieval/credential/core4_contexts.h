#ifndef CORE4_CONTEXTS_H
#define CORE4_CONTEXTS_H

#include <stdint.h>

void core4_contexts_from_digest(const uint8_t encoded[20], uint16_t contexts[2]);

#endif
