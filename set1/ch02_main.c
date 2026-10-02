#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ch02.h"

static const char ch_a[] = "1c0111001f010100061a024b53535009181c";
static const char ch_b[] = "686974207468652062756c6c277320657965";
static const char ch_out[] = "746865206b696420646f6e277420706c6179";

int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const char* a = noargs ? ch_a : argv[1];
    const char* b = noargs ? ch_b : argv[2];

    bytes_t a_raw = unhex(a);
    bytes_t b_raw = unhex(b);

    bytes_t out = fixed_xor(a_raw.bytes, a_raw.len, b_raw.bytes, b_raw.len);

    char* out_hex = hex(out.bytes, out.len);
    if (!out_hex) goto cleanup;

    printf("  a: %s\n", a);
    printf("  b: %s\n", b);
    printf("out: %s\n", out_hex);
    if (noargs) printf("exp: %s\n", ch_out);

    cleanup:
    free_bytes(&a_raw);
    free_bytes(&b_raw);
    free_bytes(&out);
    free(out_hex);
    return out_hex ? 0 : 1;
}
