#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ch09.h"

static const char ch_in[] = "YELLOW SUBMARINE";
int main(int argc, char** argv) {
    const bool noargs = argc < 2;
    const bool piped = !isatty(STDOUT_FILENO);
    const char* in_str = noargs ? ch_in : argv[1];
    bytes_t in = {
        .len = strlen(in_str)
    };
    in.bytes = malloc(in.len);
    memcpy(in.bytes, in_str, in.len);

    pkcs7_pad(&in, 20);

    char* out_ascii = try_ascii(in);
    fprintf(stderr, " in: %s\n"
                 "out: %s",
                 in_str, piped ? "(pipe)\n" : "");
    printf("%s\n", out_ascii);

    free_bytes(&in);
    free(out_ascii);
    return 0;
}