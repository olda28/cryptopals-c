#include "ch15.h"

#include <stdlib.h>


int pkcs7_strip(bytes_t* in, int blocksize) {
    const unsigned char last_byte = in->bytes[in->len-1];
    if (last_byte < 1 || last_byte > blocksize) return 0;
    for (int i = 0; i < last_byte; i++) {
        if (in->bytes[in->len - 1 - i] != last_byte) return 0;
    }

    in->len -= last_byte;
    in->bytes = realloc(in->bytes, in->len);
    return last_byte;
}
