#include <stdio.h>
#include <math.h>
#include <string.h>

#include "hash.h"

void MurMur(void * data, size_t size, hash_t seed, hash_t * HASH)
{
    uint8_t * d = (uint8_t * ) data;
    hash_t h = seed;
    hash_t k;
    for (size_t i = size >> 2; i; i--)
    {
        memcpy(&k, d, sizeof(hash_t));
        d += sizeof(hash_t);
        h ^= MurScramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0x6546b64;
    }
    k = 0;
    for (size_t i = size & 3; i; i--)
    {
        k <<= 8;
        k |= d[i - 1];
    }

    h ^= MurScramble(k);
    h ^= size;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    *HASH = h;
}

hash_t MurScramble(hash_t k)
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}
