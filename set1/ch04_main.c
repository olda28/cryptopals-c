#include "ch04.h"
#include "ch03.h"
#include <stdlib.h>
#include <stdio.h>

static const char* ch_filename = "./set1/ch04-data.txt";
static const char* ch_out = "Now that the party is jumping\n";

int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const char* filename = noargs ? ch_filename : argv[1];

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }
    crack_match_t match = find_single_xor(file, HEX);
    fclose(file);

    char* out_ascii = try_ascii(match.cracked.bytes, match.cracked.len);
    printf("    in: (%s)\n", filename);
    printf("   key: 0x%02x\n", match.cracked.key);
    printf(" score: %f\n", match.cracked.score);
    printf("  line: %d\n", match.line);
    printf("   out: %s\n", out_ascii ? out_ascii : "");
    if (noargs) printf("expect: %s\n", ch_out);

    free_crack_match(&match);
    free(out_ascii);

    return 0;
}
