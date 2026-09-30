#ifndef CH05_H
#define CH05_H
#include <stddef.h>

#include "ch01.h"

bytes_t repeat_xor(const unsigned char* in, size_t in_len, const unsigned char* key, size_t key_len);

#endif
