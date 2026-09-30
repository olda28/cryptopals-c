#include <stdio.h>
#include <stdlib.h>

#include "ch01.h"
#include "ch05.h"

static const char ch_key[] = "ICE";
static const char ch_in[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
static const char ch_out[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

int main(int argc, char** argv) {
    const char* in;
    const char* key;
    if (argc < 3) {
        key = ch_key;
        in = ch_in;
    } else {
        key = argv[1];
        in = argv[2];
    }

    const bytes_t xor = repeat_xor((unsigned char*)in, strlen(in), (unsigned char*)key, strlen(key));
    if (!xor.bytes)
        return 1;

    char* hexx = hex(xor.bytes, xor.len);
    printf("    in: %s\n", in);
    printf("   key: %s\n", key);
    printf("   out: %s\n", hexx);
    if (argc < 3) printf("expect: %s\n", ch_out);

    free(xor.bytes);
    free(hexx);
    return 0;
}