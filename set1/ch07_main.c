#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ch01.h"
#include "ch04.h"
#include "ch07.h"

static const char* ch_filename = "./set1/ch07-data.txt";
static const char* ch_key = "YELLOW SUBMARINE";
static const char* ch_out = "(./set1/ch07-expect.txt)";


int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const bool piped = !isatty(STDOUT_FILENO);
    const char* filename = noargs ? ch_filename : argv[1];
    bytes_t key = {
        .bytes = (unsigned char*)(noargs ? ch_key : argv[2]),
        .len = 0
    };
    key.len = strlen((char*)key.bytes);

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }
    file_bytes_t f = read_file(file, NEWLINE_STRIP, BASE64);
    decode_full(&f);

    bytes_t decrypted = aes128_ecb(f.decoded, key, false);

    char* out_ascii = try_ascii(decrypted);
    fprintf(stderr, "    in: (%s)\n"
                          "   key: %s\n"
                          "   out: %s"
                          "expect: %s",
                          filename, (char*)key.bytes, piped ? "(pipe)\n" : "", noargs ? ch_out: "N/A");
    printf("%s", out_ascii);

    fclose(file);
    close_file(&f);
    free_bytes(&decrypted);
    free(out_ascii);
    return 0;
}
