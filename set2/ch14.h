#ifndef SET2_CH14_H
#define SET2_CH14_H
#include "../set1/ch01.h"

bytes_t aes128_ecb_oracle_surround(bytes_t in);
void ch14_cleanup(void);
bytes_t aes128_ecb_crack_surround(bytes_t (*f)(bytes_t));

#endif
