#include "ch02.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

static const char ch_a[] = "1c0111001f010100061a024b53535009181c";
static const char ch_b[] = "686974207468652062756c6c277320657965";
static const char ch_out[] = "746865206b696420646f6e277420706c6179";

int main(int argc, char** argv){
    const char* a;
    const char* b;

    if (argc < 3){
        a = ch_a;
        b = ch_b; 
    } else {
        a = argv[1];
        b = argv[2];
    }

    char* out = fixed_xor_hex(a, b);
    if (out == NULL)
        return 1;

    printf("  a: %s\n", a);
    printf("  b: %s\n", b);
    printf("out: %s\n", out);
    if (argc < 3){
        printf("exp: %s\n", ch_out);
    }

    free(out);
    return 0;
}
