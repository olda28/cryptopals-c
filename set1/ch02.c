#include "ch01.h"
#include "ch02.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bytes_t fixed_xor(const unsigned char* a, size_t a_len, const unsigned char* b, size_t b_len) {
    if (a_len != b_len || a_len == 0 || b_len == 0)
        return NO_BYTES;

    const bytes_t out = { .bytes = malloc(a_len), .len = a_len };
    for (size_t i = 0; i < a_len; i++) {
        out.bytes[i] = a[i] ^ b[i];
    }
    return out;
}
