#include "ch01.h"
#include "ch02.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bytes_t fixed_xor(const unsigned char* a, size_t a_len, const unsigned char* b, size_t b_len) {
    if (a_len != b_len)
        return NO_BYTES;

    const bytes_t out = { .bytes = malloc(a_len), .len = a_len };

    for (size_t i = 0; i < a_len; i++) {
        out.bytes[i] = a[i] ^ b[i];
    }

    return out;
}

char* fixed_xor_hex(const char* a_hex, const char* b_hex) {
    const size_t a_len = strlen(a_hex);
    const size_t b_len = strlen(b_hex);
    if (a_len != b_len)
        return NULL;

    const bytes_t a = unhex(a_hex);
    if (!a.bytes)
        return NULL;

    const bytes_t b = unhex(b_hex);
    if (!b.bytes) {
        free(a.bytes);
        return NULL;
    }

    const bytes_t xor = fixed_xor(a.bytes, a.len, b.bytes, b.len);
    free(a.bytes);
    free(b.bytes);
    if (!xor.bytes)
        return NULL;

    char* out_hex = hex(xor.bytes, xor.len);
    free(xor.bytes);

    return out_hex;
}
