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
    } else {
        xoshiross_bytes(dest, len);
        return 0;
    }
}