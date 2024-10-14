#ifndef HASH_H
#define HASH_H

#include "stack.h"
#include <stdint.h>

typedef uint32_t hash_t;

void MurMur(void * data, size_t size, hash_t seed, hash_t * HASH);
hash_t MurScramble(hash_t k);

static hash_t data_hash = 0;
static hash_t stack_hash = 0;
const static hash_t SEED = 1234566666;

#endif
