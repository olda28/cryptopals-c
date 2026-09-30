#include "ch04.h"

#include "ch03.h"
#include <stdio.h>
#include <stdlib.h>


crack_match_hex_t find_single_xor_hex(FILE* file, size_t max_line_len) {
    crack_match_hex_t solution = {
        .cracked = {
            .bytes = NULL,
            .key = 0x00,
            .score = 900.0
        },
        .line = 0
    };

    int line_nr = 1;
    char* line = malloc(max_line_len + 2); // +1 newline, +1 nul
    while (fgets(line, (int)max_line_len + 2, file)) {
        line[strcspn(line, "\n")] = '\0';
        const crack_result_hex_t attempt = single_xor_crack_hex(line);

        if (attempt.bytes) {
            if (attempt.score < solution.cracked.score) {
                if (solution.cracked.bytes) free(solution.cracked.bytes);
                solution.cracked = attempt;
                solution.line = line_nr;
            }
            else {
                free(attempt.bytes);
            }
        }
        line_nr++;
    }
    free(line);

    return solution;
}
