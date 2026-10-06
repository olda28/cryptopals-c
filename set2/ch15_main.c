#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ch15.h"

static unsigned char ch_in[16] = "ICE ICE BABY\004\004\004\004";
int main(int argc, char** argv) {
    unsigned char* in = argc < 2 ? ch_in : (unsigned char*)argv[1];

    const size_t in_len = strlen((char*)in);
    if (in_len % 16 != 0) return 1;

    bytes_t in_bytes = { .bytes = malloc(in_len), .len = in_len };
    memcpy(in_bytes.bytes, in, in_len); // pkcs7 reallocs, can't be outside heap

    char* in_ascii = try_ascii(in_bytes);

    int count;
    if ((count = pkcs7_strip(&in_bytes, 16)) == 0) {
        fprintf(stderr, "pkcs7_strip: failed to strip valid padding\n");
        return 1;
    }

    char* stripped_ascii = try_ascii(in_bytes);
    printf("in: %s\n"
        "stripped %d bytes\n"
        "out: %s\n",
        in_ascii, count, stripped_ascii);

    free(stripped_ascii);
}