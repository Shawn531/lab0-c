#ifndef _QRANDOM_H
#define _QRANDOM_H

#include <stddef.h>
#include <stdint.h>

extern int qrandom_impl;
int qrandombytes(uint8_t *buf, size_t len);

#endif