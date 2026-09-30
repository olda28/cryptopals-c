#ifndef CH06_H
#define CH06_H
#include <stddef.h>

typedef struct {
    unsigned char* bytes;
    size_t len;
    unsigned char* key;
    size_t key_len;
} repeat_crack_result_t;

#define REPEAT_CRACK_FAIL (repeat_crack_result_t){ NULL, 0, NULL, 0}

repeat_crack_result_t repeat_xor_crack(unsigned char* in, size_t in_len);

#endif
