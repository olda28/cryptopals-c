#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch01.h"
#include "ch06.h"

static const char* ch_filename = "./set1/ch06-data.txt";
static const char* ch_out = "?";

static char* readfile_strip_nl(FILE* file, size_t max_len) {
    char* buf = malloc(max_len + 1);
    char* w = buf;
    int c;
    while ((c = getc(file)) != EOF && (size_t)(w - buf) < max_len - 1) {
        if (c == '\n') continue;
        *w++ = (char)c;
    }
    *w = '\0';
    return buf;
}

int main(int argc, char** argv) {
    const char* filename = ch_filename;
    const size_t max_len = 5000;

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }
    char* base = readfile_strip_nl(file, max_len);
    fclose(file);

    printf("file: %lu\n", strlen(base));
    const bytes_t raw = unbase64(base);
    free(base);
    if (!raw.bytes)
        return 1;

    const repeat_crack_result_t crack = repeat_xor_crack(raw.bytes, raw.len);
    free(raw.bytes);
    printf("key len: %lu, key: ", crack.key_len);
    for (unsigned int i = 0; i < crack.key_len; i++) {
        printf("%c", (char)crack.key[i]);
    }
    printf("\n");
    printf("bytelen: %lu, bytes: ", crack.len);
    for (unsigned int i = 0; i < crack.len; i++) {
        printf("%c", (char)crack.bytes[i]);
    }
    printf("\n");
    free(crack.key);

    if (!crack.bytes)
        return 1;
    /*
    printf("    in: (%s)\n", filename);
    printf("   key: 0x%02x\n", match.cracked.key);
    printf(" score: %f\n", match.cracked.score);
    printf("  line: %d\n", match.line);
    printf("   out: %s\n", match.cracked.bytes);

    if (argc < 3)
        printf("expect: %s\n", ch_out);*/
    return 0;
}
