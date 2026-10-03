#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ch02.h"

static const char ch_a[] = "1c0111001f010100061a024b53535009181c";
static const char ch_b[] = "686974207468652062756c6c277320657965";
static const char ch_out[] = "746865206b696420646f6e277420706c6179";

int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const char* a_hex = noargs ? ch_a : argv[1];
    const char* b_hex = noargs ? ch_b : argv[2];

    bytes_t a = unhex(a_hex);
    bytes_t b = unhex(b_hex);

    bytes_t out = fixed_xor(a, b);

    char* out_hex = hex(out);

    printf(
    "  a: %s\n"
          "  b: %s\n"
          "out: %s\n",
          a_hex, b_hex, out_hex);
    if (noargs) printf("exp: %s\n", ch_out);

    free_bytes(&a);
    free_bytes(&b);
    free_bytes(&out);
    free(out_hex);
    return out_hex ? 0 : 1;
}
