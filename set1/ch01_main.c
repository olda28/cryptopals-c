#include <stdbool.h>

#include "ch01.h"
#include <stdlib.h>
#include <stdio.h>

static const char ch_in[] =
    "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
static const char ch_out[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

int main(int argc, char** argv) {
    const bool noargs = argc < 2;
    const char* hex = noargs ? ch_in : argv[1];

    bytes_t raw = unhex(hex);
    char* base = base64(raw);

    printf(
    "    in: %s\n"
          "   out: %s\n",
          hex, base);
    if (noargs) printf("expect: %s\n", ch_out);

    free_bytes(&raw);
    free(base);

    return 0;
}
