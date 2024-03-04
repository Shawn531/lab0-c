#ifndef _XOSHIRO_H
#define _XOSHIRO_H

#include <stddef.h>
#include <stdint.h>

void s4_seed(const uint64_t seed);
void s2_seed(const uint64_t seed);
void s_seed(const uint64_t seed);
uint64_t xoshiro(void);
void xoshiro_bytes(uint8_t *dest, const size_t len);
uint64_t xoshiro256ss(void);
void xoshiross_bytes(uint8_t *dest, const size_t len);
uint64_t xorshift128p(void);
void xorshift128p_bytes(uint8_t *dest, const size_t len);
uint64_t xorshift64(void);
void xorshift64_bytes(uint8_t *dest, const size_t len);

#endif