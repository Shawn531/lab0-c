/*
 *  Written by David Blackman and Sebastiano Vigna (vigna@acm.org)
 *  Modified by Yan-Jie Wang (yanjiewtw@gmail.com)
 *
 *  To the extent possible under law, the author has dedicated all copyright
 *  and related and neighboring rights to this software to the public domain
 *  worldwide. This software is distributed without any warranty.
 *
 *  See <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/*
 * Imported from https://prng.di.unimi.it/splitmix64.c
 * and https://prng.di.unimi.it/xoshiro256plusplus.c
 */

#include "prng.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* splitmix64 for initialization */
static uint64_t splitmix64_s = 0;

static uint64_t s_4[4];
static uint64_t s_2[2];
static uint64_t s;
static bool seeded = false;

static void splitmix64_seed(const uint64_t seed)
{
    splitmix64_s = seed;
}

static uint64_t splitmix64()
{
    uint64_t z = (splitmix64_s += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

void s4_seed(const uint64_t seed)
{
    seeded = true;
    splitmix64_seed(seed);
    for (int i = 0; i < 4; i++)
        s_4[i] = splitmix64();
}

void s2_seed(const uint64_t seed)
{
    seeded = true;
    splitmix64_seed(seed);
    for (int i = 0; i < 2; i++)
        s_2[i] = splitmix64();
}

void s_seed(const uint64_t seed)
{
    seeded = true;
    splitmix64_seed(seed);
    s = splitmix64();
}

static inline uint64_t rotl(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}


/* xoshiro+ */
uint64_t xoshiro(void)
{
    if (!seeded)
        s4_seed(0);

    const uint64_t result = rotl(s_4[0] + s_4[3], 23) + s_4[0];
    const uint64_t t = s_4[1] << 17;
    s_4[2] ^= s_4[0];
    s_4[3] ^= s_4[1];
    s_4[1] ^= s_4[2];
    s_4[0] ^= s_4[3];
    s_4[2] ^= t;
    s_4[3] = rotl(s_4[3], 45);
    return result;
}

void xoshiro_bytes(uint8_t *dest, const size_t len)
{
    uint64_t tmp;
    for (size_t i = 0; i < len - (len % 8); i += 8) {
        tmp = xoshiro();
        memcpy(&dest[i], &tmp, 8);
    }

    if (len % 8) {
        tmp = xoshiro();
        memcpy(&dest[len - (len % 8)], &tmp, len % 8);
    }
}

/* xoshiro256** */

uint64_t xoshiro256ss()
{
    if (!seeded)
        s4_seed(0);

    uint64_t const result = rotl(s_4[1] * 5, 7) * 9;
    uint64_t const t = s_4[1] << 17;

    s_4[2] ^= s_4[0];
    s_4[3] ^= s_4[1];
    s_4[1] ^= s_4[2];
    s_4[0] ^= s_4[3];

    s_4[2] ^= t;
    s_4[3] = rotl(s_4[3], 45);

    return result;
}

void xoshiross_bytes(uint8_t *dest, const size_t len)
{
    uint64_t tmp;
    for (size_t i = 0; i < len - (len % 8); i += 8) {
        tmp = xoshiro256ss();
        memcpy(&dest[i], &tmp, 8);
    }

    if (len % 8) {
        tmp = xoshiro256ss();
        memcpy(&dest[len - (len % 8)], &tmp, len % 8);
    }
}

/* xorshift128p */
uint64_t xorshift128p()
{
    if (!seeded) {
        s2_seed(0);
    }
    uint64_t sp = s_2[0];
    uint64_t const t = s_2[1];
    s_2[0] = t;
    sp ^= sp << 23;  // a
    sp ^= sp >> 18;  // b -- Again, the shifts and the multipliers are tunable
    sp ^= t ^ (t >> 5);  // c
    s_2[1] = sp;
    return sp + t;
}

void xorshift128p_bytes(uint8_t *dest, const size_t len)
{
    uint64_t tmp;
    for (size_t i = 0; i < len - (len % 8); i += 8) {
        tmp = xorshift128p();
        memcpy(&dest[i], &tmp, 8);
    }

    if (len % 8) {
        tmp = xorshift128p();
        memcpy(&dest[len - (len % 8)], &tmp, len % 8);
    }
}

/* xorshift64 */
uint64_t xorshift64()
{
    if (!seeded) {
        s_seed(0);
    }
    uint64_t x = s;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return s = x;
}

void xorshift64_bytes(uint8_t *dest, const size_t len)
{
    uint64_t tmp;
    for (size_t i = 0; i < len - (len % 8); i += 8) {
        tmp = xorshift64();
        memcpy(&dest[i], &tmp, 8);
    }

    if (len % 8) {
        tmp = xorshift64();
        memcpy(&dest[len - (len % 8)], &tmp, len % 8);
    }
}