#include "ch04.h"
#include "ch03.h"
#include <stdlib.h>
#include <stdio.h>

#include "ch08.h"

static const char* ch_filename = "./set1/ch08-data.txt";
static const char* ch_out = "(hex) d880619740a8a19b7840a8a31c810a3d08649af70dc06f4fd5d2d69c744cd283e2dd052f6b641dbf9d11b0348542bb5708649af70dc06f4fd5d2d69c744cd2839475c9dfdbc1d46597949d9c7e82bf5a08649af70dc06f4fd5d2d69c744cd28397a93eab8d6aecd566489154789a6b0308649af70dc06f4fd5d2d69c744cd283d403180c98c8f6db1f2a3f9c4040deb0ab51b29933f2c123c58386b06fba186a";

int main(int argc, char** argv) {
    const bool noargs = argc < 3;
    const char* filename = noargs ? ch_filename : argv[1];

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: file '%s' not found.\n", filename);
        return 1;
    }
    find_match_t found = find_aes128_ecb(file, HEX);
    fclose(file);

    char* out_ascii = try_ascii(found.bytes.bytes, found.bytes.len);
    printf("    in: (%s)\n", filename);
    printf("  line: %d\n", found.line);
    printf(" bytes: %s\n", out_ascii);
    if (noargs) printf("expect: %s\n", ch_out);

    free_find_match(&found);
    free(out_ascii);
    return 0;
}
