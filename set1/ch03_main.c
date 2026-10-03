#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include "ch03.h"

static const char ch_in[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
static const char ch_out[] = "Cooking MC's like a pound of bacon";

int main(int argc, char** argv) {
    const bool noargs = argc < 2;
    const char* in_hex = noargs ? ch_in : argv[1];

    bytes_t in = unhex(in_hex);
    crack_result_t cracked = single_xor_crack(in);
    char* out_ascii = try_ascii((bytes_t){ .bytes = cracked.bytes, .len = cracked.len });

    printf(
     "    in: %s\n"
           "   key: 0x%02x\n"
           " score: %f\n"
           "   out: %s\n", in_hex, cracked.key, cracked.score, out_ascii);
    if (noargs) printf("expect: %s\n", ch_out);

    free_bytes(&in);
    free_crack_result(&cracked);
    free(out_ascii);

    return 0;
}
