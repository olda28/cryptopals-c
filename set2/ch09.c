#include "ch09.h"

#include <stdlib.h>

#include "../set1/ch01.h"

void pkcs7_pad(bytes_t* in, int blocksize) {
    int padding_len = blocksize - in->len;
    if (padding_len == 0) padding_len = blocksize;

    in->bytes = realloc(in->bytes, in->len + padding_len);

    for (int i = 0; i < padding_len; i++) {
        in->bytes[in->len + i] = (unsigned char)padding_len;
    }

    in->len += padding_len;
}