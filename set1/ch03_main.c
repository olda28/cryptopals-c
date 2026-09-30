#include "ch03.h"
#include <stdlib.h>
#include <stdio.h>

static const char ch_in[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
static const char ch_out[] = "Cooking MC's like a pound of bacon";
int main(int argc, char** argv){
    const char* hex;
    if (argc < 2)
        hex = ch_in;
    else
        hex = argv[1];
    printf("    in: %s\n", hex);


    const crack_result_hex_t cracked = single_xor_crack_hex(hex);
    if (!cracked.bytes)
        return 1;

    printf("   key: 0x%02x\n", cracked.key);
    printf(" score: %f\n", cracked.score);
    printf("   out: %s\n", cracked.bytes);
    
    if (argc < 2)
        printf("expect: %s\n", ch_out);

    free(cracked.bytes);

    return 0;
}
