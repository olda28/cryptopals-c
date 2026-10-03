#ifndef SET1_CH03_H
#define SET1_CH03_H

#include <stddef.h>
#include "ch01.h"

typedef struct {
    unsigned char* bytes;
    size_t len;
    unsigned char key;
    double score;
} crack_result_t;
#define CRACK_FAIL (crack_result_t){ NULL, 0, 0x00, -1.0 }
void free_crack_result(crack_result_t* crack_result);

bytes_t single_xor(bytes_t in, unsigned char key);
crack_result_t single_xor_crack(bytes_t in);


#endif
