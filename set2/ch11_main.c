#include <stdio.h>
#include <stdlib.h>

#include "ch11.h"
#include "../set1/ch07.h"





int main(void) {    
    const bool ecb = is_aes128_ecb(aes128_random_mode, 16);
    printf("Detected: %s\n", ecb ? "ECB" : "CBC");
}