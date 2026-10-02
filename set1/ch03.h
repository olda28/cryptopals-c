#ifndef CH03_H
#define CH03_H

#include <stddef.h>
#include "ch01.h"

typedef struct {
    unsigned char* bytes;
    size_t len;
    unsigned char key;
    double score;
} crack_result_t;
#define CRACK_FAIL (crack_result_t){ NULL, 0, 0x00, -1.0 }

bytes_t single_xor(const unsigned char* in, size_t in_len, unsigned char key);
crack_result_t single_xor_crack(const unsigned char* in, size_t in_len);


#endif
