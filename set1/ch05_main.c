#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ch01.h"
#include "ch05.h"

static const char ch_key[] = "ICE";
static const char ch_in[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
static const char ch_out[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const char* key = noargs ? ch_key : argv[1];
    const char* in = noargs ? ch_in : argv[2];

    bytes_t out = repeat_xor((bytes_t){ (unsigned char*) in, strlen(in) },
        (bytes_t){ (unsigned char*)key, strlen(key) });

    char* out_hex = hex(out);
    printf("    in: %s\n"
                 "   key: %s\n"
                 "   out: %s\n",
                 in, key, out_hex);
    if (noargs) printf("expect: %s\n", ch_out);

    free_bytes(&out);
    free(out_hex);
    return 0;
}