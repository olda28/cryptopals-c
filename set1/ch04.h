#ifndef CH04_H
#define CH04_H

#include "ch03.h"
#include <stdio.h>

typedef struct {
    crack_result_hex_t cracked;
    int line;
} crack_match_hex_t;

#define NO_MATCH (crack_match_hex_t){ CRACK_FAIL_HEX, 0 }

/* Attempts to find a single-byte-xor-crackable line in a file.
 * Works on a per-line basis (steps at newline)
 * Max-line len should be the amount of characters per line (excluding newline) */
crack_match_hex_t find_single_xor_hex(FILE* file, size_t max_line_len);

#endif
