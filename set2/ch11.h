#ifndef SET2_CH11_H
#define SET2_CH11_H
#include <stdbool.h>

#include "../set1/ch01.h"

bytes_t repeat_char(unsigned char c, int len);
bytes_t random_bytes(size_t len);
int randint(int min, int max);

bytes_t aes128_random_mode(bytes_t in);
bool is_aes128_ecb(bytes_t (*f)(bytes_t), int blocksize);

#endif
