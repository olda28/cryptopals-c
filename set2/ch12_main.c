#include <stdio.h>
#include <stdlib.h>

#include "ch12.h"

int main(void) {
    bytes_t cracked = aes128_ecb_crack_append(aes128_ecb_oracle_append);

    char* ascii_cracked = try_ascii(cracked);
    printf("cracked: %s", ascii_cracked);

    free_bytes(&cracked);
    free(ascii_cracked);
    ch12_cleanup(); // free global variables
    return 0;
}