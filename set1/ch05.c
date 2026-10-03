#include "ch05.h"

#include <stdlib.h>
#include "ch01.h"

bytes_t repeat_xor(const bytes_t in, const bytes_t key) {
    if (in.len == 0) return NO_BYTES;
    const bytes_t out = {
        .bytes = malloc(in.len),
        .len = in.len
    };

    for (size_t i = 0; i < in.len; i++) {
        const int j = i % key.len;
        out.bytes[i] = in.bytes[i] ^ key.bytes[j];
    }
    return out;
}
