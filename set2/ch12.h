#ifndef SET2_CH12_H
#define SET2_CH12_H
#include "../set1/ch01.h"

bytes_t aes128_ecb_crackme(bytes_t in);
void aes128_ecb_crackme_cleanup(void);

bytes_t aes128_ecb_crack(bytes_t (*f)(bytes_t));

#endif
