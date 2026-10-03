#ifndef SET1_CH06_H
#define SET1_CH06_H
#include <stddef.h>
#include "ch01.h"

typedef struct {
    unsigned char* bytes;
    size_t len;
    unsigned char* key;
    size_t key_len;
} repeat_crack_result_t;
#define REPEAT_CRACK_FAIL (repeat_crack_result_t){ NULL, 0, NULL, 0}
void free_repeat_crack_result_t(repeat_crack_result_t* repeat_crack_result);

repeat_crack_result_t repeat_xor_crack(bytes_t in);

#endif
