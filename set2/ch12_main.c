#include <stdio.h>
#include <stdlib.h>

#include "ch12.h"

int main(void) {
    bytes_t cracked = aes128_ecb_crack(aes128_ecb_crackme);
    aes128_ecb_crackme_cleanup();

    char* ascii_cracked = try_ascii(cracked);
    printf("cracked: %s", ascii_cracked);
    free_bytes(&cracked);
    free(ascii_cracked);
    return 0;
}