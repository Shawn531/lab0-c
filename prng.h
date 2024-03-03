#ifndef _XOSHIRO_H
#define _XOSHIRO_H

#include <stddef.h>
#include <stdint.h>

void xoshiro_bytes(uint8_t *dest, const size_t len);
void xoshiro_seed(const uint64_t seed);
uint64_t xoshiro(void);
uint64_t xoshiro256ss(void);
void xoshiross_bytes(uint8_t *dest, const size_t len);
uint64_t xorshift128p(void);
void xorshift128p_bytes(uint8_t *dest, const size_t len);
void xorshift128_seed(const uint64_t seed);


#endif