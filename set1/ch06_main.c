#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ch01.h"
#include "ch04.h"
#include "ch06.h"

static const char* ch_filename = "./set1/ch06-data.txt";
static const char* ch_out = "(./set1/ch06-expect.txt)";


int main(int argc, char** argv) {
    const bool noargs = argc < 2;
    const bool piped = !isatty(STDOUT_FILENO);
    const char* filename = noargs ? ch_filename : argv[1];;

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }
    file_bytes_t f = read_file(file, NEWLINE_STRIP, BASE64);
    decode_full(&f);

    const repeat_crack_result_t crack = repeat_xor_crack(f.decoded.bytes, f.decoded.len);

    char* key_ascii = try_ascii(crack.key, crack.key_len);
    char* out_ascii = try_ascii(crack.bytes, crack.len);
    fprintf(stderr, "    in: (%s)\n", filename);
    fprintf(stderr, "   key: %s\n", key_ascii);
    fprintf(stderr, "   out: %s\n", piped ? "(pipe)" : out_ascii);
    fprintf(stderr, "expect: %s\n", ch_out);
    printf("%s", out_ascii);

    fclose(file);
    close_file(&f);
    free(crack.bytes);
    free(crack.key);
    free(key_ascii);
    free(out_ascii);
    return 0;
}
