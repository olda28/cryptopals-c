#include "ch05.h"

#include <stdlib.h>
#include "ch01.h"

bytes_t repeat_xor(const unsigned char* in, size_t in_len, const unsigned char* key, size_t key_len) {
    const bytes_t out = {
        .bytes = malloc(in_len),
        .len = in_len
    };

    for (size_t i = 0; i < in_len; i++) {
        const int j = i % key_len;
        out.bytes[i] = in[i] ^ key[j];
    }
    return out;
}
