#ifndef SET2_CH10_H
#define SET2_CH10_H
#include <stdbool.h>

#include "../set1/ch01.h"

bytes_t aes128_cbc(bytes_t in, bytes_t key, bytes_t iv, bool encrypt);
#endif
