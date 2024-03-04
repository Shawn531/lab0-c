#include "qrandom.h"
#include <limits.h>
#include "prng.h"
#include "random.h"

int qrandom_impl = 0;

int qrandombytes(uint8_t *dest, const size_t len)
{
    if (qrandom_impl == 0) {
        return randombytes(dest, len);
    } else if (qrandom_impl == 1) {
        xoshiro_bytes(dest, len);
        return 0;
    } else if (qrandom_impl == 2) {
        xoshiross_bytes(dest, len);
        return 0;
    } else if (qrandom_impl == 3) {
        xorshift64_bytes(dest, len);
        return 0;
    } else {
        xorshift128p_bytes(dest, len);
        return 0;
    }
}