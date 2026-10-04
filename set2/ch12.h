#ifndef SET2_CH12_H
#define SET2_CH12_H
#include "../set1/ch01.h"

bytes_t aes128_ecb_oracle_append(bytes_t in);
void ch12_cleanup(void);
bytes_t aes128_ecb_crack_append(bytes_t (*f)(bytes_t));

#endif
