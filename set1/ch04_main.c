#include "ch04.h"
#include "ch03.h"
#include <stdlib.h>
#include <stdio.h>

static const char* ch_filename = "./ch04-data.txt";
static const size_t ch_max_line_len = 60;
static const char* ch_out = "Now that the party is jumping\n";

int main(int argc, char** argv) {
    const char* filename;
    size_t max_line_len;
    if (argc < 3) {
        filename = ch_filename;
        max_line_len = ch_max_line_len;
    }
    else {
        filename = argv[1];
        max_line_len = atoi(argv[2]);
    }

    if (max_line_len == 0) {
        fprintf(stderr, "Error: invalid max line length '%ld'  was provided.\n", max_line_len);
        return 1;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }

    crack_match_hex_t match = find_single_xor_hex(file, 60);
    fclose(file);
    if (!match.cracked.bytes) {
        return 1;
    }

    printf("    in: (%s)\n", filename);
    printf("   key: 0x%02x\n", match.cracked.key);
    printf(" score: %f\n", match.cracked.score);
    printf("  line: %d\n", match.line);
    printf("   out: %s\n", match.cracked.bytes);

    if (argc < 3)
        printf("expect: %s\n", ch_out);

    free(match.cracked.bytes);

    return 0;
}
