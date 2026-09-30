#include "ch01.h"
#include <stdlib.h>
#include <stdio.h>

static const char ch_in[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
static const char ch_out[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
int main(int argc, char** argv){
    const char* hex;
    if (argc < 2)
        hex = ch_in;
    else
        hex = argv[1];
    printf("    in: %s\n", hex);

    bytes_t raw = unhex(hex);
    if (raw.bytes == NULL)
        return 1;

    printf("   raw: ");
    for (size_t i = 0; i < raw.len; i++){
        printf("0x%02x ", raw.bytes[i]);
    }
    printf("\n");

    char* base = base64(raw.bytes, raw.len);
    if (base == NULL){
        free(raw.bytes);
        return 1;
    }

    printf("base64: %s\n", base);
    if (argc < 2)
        printf("expect: %s\n", ch_out);

    free(raw.bytes);
    free(base);

    return 0;
}
