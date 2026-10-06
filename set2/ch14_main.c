#include <stdio.h>
#include <stdlib.h>

#include "ch14.h"

int main(void) {
    bytes_t cracked = aes128_ecb_crack_surround(aes128_ecb_oracle_surround);

    char* ascii_cracked = try_ascii(cracked);
    printf("cracked: %s", ascii_cracked);

    free_bytes(&cracked);
    free(ascii_cracked);
    ch14_cleanup(); // free global variables
    return 0;
}