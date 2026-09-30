#ifndef CH02_H
#define CH02_H

#include <stddef.h>

char* fixed_xor_hex(const char* a, const char* b);
unsigned char* fixed_xor_raw(const unsigned char* a, size_t a_len, const unsigned char* b, size_t b_len);

#endif
