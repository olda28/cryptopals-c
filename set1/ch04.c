#include "ch04.h"

#include <stdio.h>
#include <stdlib.h>
#include "ch03.h"

void free_crack_match(crack_match_t* crack_match) {
    free_crack_result(&crack_match->cracked);
}

/**
 * Read a file, treating newlines as specified, with a given encoding.
 * @warning The returned value's .bytes_start must be freed.
 */
file_bytes_t read_file(FILE* file, READFILE_NEWLINE newlines, ENCODING encoding) {
    size_t buf_len = 8192;
    char* buf = malloc(buf_len);
    char* w = buf;
    int c;
    while ((c = getc(file)) != EOF) {
        const size_t write_offset = (size_t)(w-buf);
        if (write_offset == buf_len) {
            buf_len *= 2;
            char* newbuf = realloc(buf, buf_len);
            if (!newbuf) {
                free(buf);
                return NO_FILE_BYTES;
            }
            buf = newbuf;
            w = newbuf+write_offset;
        }
        if (c == '\n') {
            if (newlines == NEWLINE_STRIP) continue;
            if (newlines == NEWLINE_REPLACE_NUL) c = '\0';
        }
        *w++ = (char)c;
    }
    *w = '\0';

    return (file_bytes_t){ .bytes_start = buf, .bytes = buf, .len = w-buf, .decoded = NO_BYTES, .encoding = encoding };
}

/**
 *
 * Return the next decoded line of a read file.
 * @param f pointer to file_bytes returned by read_file(..., NEWLINE_REPLACE_NUL, ...)
 * @warning f.decoded must be freed.
 * @return Boolean indicating whether there is a next line.
 */
bool decode_nextline(file_bytes_t* f) {
    if ((size_t)(f->bytes - f->bytes_start) >= f->len) {
        f->decoded.bytes = NULL;
        f->decoded.len = 0;
        return false;
    }
    const char* to_return = f->bytes;
    f->bytes += strlen(f->bytes) + 1;
    f->decoded = decode(to_return, f->encoding);
    return f->decoded.len > 0;
}

/**
 * Return the full decoded buffer of a read file.
 * @warning f.decoded must be freed.
 */
void decode_full(file_bytes_t* f) {
    f->decoded = decode(f->bytes_start, f->encoding);
}

void close_file(file_bytes_t* f) {
    free(f->bytes_start);
    free(f->decoded.bytes);
    *f = NO_FILE_BYTES;
}

crack_match_t find_single_xor(FILE* file, ENCODING file_encoding) {
    crack_match_t solution = {
        .cracked = {
            .bytes = NULL,
            .key = 0x00,
            .score = 900.0
        },
        .line = 0
    };

    int line_nr = 1;

    file_bytes_t f = read_file(file, NEWLINE_REPLACE_NUL, file_encoding);
    while (decode_nextline(&f)){
        crack_result_t attempt = single_xor_crack(f.decoded);

        if (attempt.bytes) {
            if (attempt.score < solution.cracked.score) {
                free(solution.cracked.bytes);
                solution.cracked = attempt;
                solution.line = line_nr;
            } else {
                free_crack_result(&attempt);
            }
        }
        line_nr++;
        free_bytes(&f.decoded);
    }

    free(f.bytes_start);
    return solution;
}
