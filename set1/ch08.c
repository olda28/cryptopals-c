#include "ch08.h"

#include <stdio.h>
#include <stdlib.h>

#include "ch04.h"

void free_find_match(find_match_t* match) {
    free(match->bytes.bytes);
    match->bytes.bytes = NULL;
}

static int count_duplicate_blocks(const bytes_t bytes, size_t block_len) {
    int count = 0;
    const size_t blocks_n = bytes.len/block_len;
    for (size_t i = 0; i < blocks_n; i++) {
        const unsigned char* blk_1 = bytes.bytes+i*block_len;
        for (size_t j = i+1; j < blocks_n; j++) {
            const unsigned char* blk_2 = bytes.bytes+j*block_len;
            if (memcmp(blk_1, blk_2, block_len) == 0) count++;
        }
    }
    return count;
}

line_bytes_t find_aes128_ecb(FILE* file, ENCODING file_encoding) {
    line_bytes_t found = {
        .bytes = NULL,
        .len = 0,
        .line = 0
    };

    int line_nr = 1;
    int duplicates_max = 0;

    file_bytes_t f = read_file(file, NEWLINE_REPLACE_NUL, file_encoding);
    if (!f.bytes_start) {
        fprintf(stderr, "Failed to decode file.");
        return NO_LINE_BYTES;
    }
    while (decode_nextline(&f)) {
        const int duplicates = count_duplicate_blocks(f.decoded, 16);
        if (duplicates > duplicates_max) {
            free(found.bytes);
            found.bytes = f.decoded.bytes;
            found.len = f.decoded.len;
            found.line = line_nr;
            duplicates_max = duplicates;
        } else {
            free(f.decoded.bytes);
        }
        line_nr++;
    }

    free(f.bytes_start);
    return found;
}
