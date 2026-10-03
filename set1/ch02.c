#include "ch01.h"
#include "ch02.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bytes_t fixed_xor(bytes_t a, bytes_t b) {
    if (a.len != b.len || a.len == 0 ||b .len == 0)
        return NO_BYTES;

    const bytes_t out = { .bytes = malloc(a.len), .len = a.len };
    for (size_t i = 0; i < a.len; i++) {
        out.bytes[i] = a.bytes[i] ^ b.bytes[i];
    }
    return out;
}
