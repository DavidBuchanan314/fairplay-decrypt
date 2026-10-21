#ifndef CORE4_SEED_H
#define CORE4_SEED_H

#include <stdint.h>

int core4_seed(uint16_t context, uint8_t output[38]);
int core4_seeds_from_digest(const uint8_t encoded[20], uint8_t first[38], uint8_t second[38]);

#endif
