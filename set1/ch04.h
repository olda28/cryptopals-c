#ifndef CH04_H
#define CH04_H

#include <stdbool.h>

#include "ch03.h"
#include <stdio.h>

typedef enum HANDLE_NEWLINES {
    NEWLINE_KEEP,
    NEWLINE_STRIP,
    NEWLINE_REPLACE_NUL
} READFILE_NEWLINE;

typedef struct {
    char* bytes_start;
    size_t len;
    ENCODING encoding;
    char* bytes;
    bytes_t decoded;
} file_bytes_t;
#define NO_FILE_BYTES (file_bytes_t){ .bytes_start = NULL, .bytes = NULL, .len = 0, .decoded = NO_BYTES, .encoding = NONE };


typedef struct {
    crack_result_t cracked;
    int line;
} crack_match_t;
#define NO_MATCH (crack_match_t){ CRACK_FAIL, 0 }
void free_crack_match(crack_match_t* crack_match);

file_bytes_t read_file(FILE* file, READFILE_NEWLINE newlines, ENCODING encoding);
bool decode_nextline(file_bytes_t* f);
void decode_full(file_bytes_t* f);
void close_file(file_bytes_t* f);


/* Attempts to find a single-byte-xor-crackable line in a file.
 * Works on a per-line basis (steps at newline)
 * Max-line len should be the amount of characters per line (excluding newline) */
crack_match_t find_single_xor(FILE* file, ENCODING file_encoding);

#endif
