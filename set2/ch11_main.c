#include <stdio.h>
#include <stdlib.h>

#include "ch11.h"
#include "../set1/ch07.h"

static bytes_t repeat_char(unsigned char c, int len) {
    bytes_t out = {
        .bytes = malloc(len),
        .len = len
    };
    for (int i = 0; i < len; i++) {
        out.bytes[i] = c;
    }
    return out;
}



int main(void) {
    bytes_t in = repeat_char('X', 48);

    bytes_t random = aes128_random(in, true);
    const bool ecb = !memcmp(random.bytes+16, random.bytes+32, 16);
    printf("Detected: %s\n", ecb ? "ECB" : "CBC");

    free_bytes(&in);
    free_bytes(&random);
}