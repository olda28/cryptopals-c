#ifndef CH03_H
#define CH03_H

#include "ch01.h"
#include <stddef.h>

/* Performs xor using a single repeating character
 * Returns a newly allocated buffer */
bytes_t single_xor(const unsigned char* in, size_t in_len, unsigned char key);

/* Attempts to find the single-byte key for a single_xor decryption
 * Assumes the decrypted bytes are an ASCII string of English text
 * Returns the decrypted bytes if successful
 * Stores the found key in byte
 * Returns a newly allocated buffer */
typedef struct {
    unsigned char* bytes;
    size_t len;
    unsigned char key;
    double score;
} crack_result_t;

#define CRACK_FAIL (crack_result_t){ NULL, 0, 0x00, -1.0 }

typedef struct {
    char* bytes;
    unsigned char key;
    double score;
} crack_result_hex_t;

#define CRACK_FAIL_HEX (crack_result_hex_t){ NULL, 0x00, -1.0 }

crack_result_t single_xor_crack(const unsigned char* in, size_t in_len);
crack_result_hex_t single_xor_crack_hex(const char* hex);


#endif
