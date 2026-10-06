#ifndef SET2_CH15_H
#define SET2_CH15_H

#include "../set1/ch01.h"

/** @returns the amount of padding bytes stripped, or zero on invalid padding. */
int pkcs7_strip(bytes_t* in, int blocksize);

#endif
