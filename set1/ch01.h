#ifndef CH01_H
#define CH01_H
#include <string.h>

typedef struct {
    unsigned char* bytes;
    size_t len; 
} bytes_t;
#define NO_BYTES ((bytes_t){ NULL, 0 })

/* Converts a NUL-terminated hex string to raw bytes.
 * Returns a newly allocated buffer.
 * If strlen(hex) is not even or 0, returns NULL */
bytes_t unhex(const char* hex);

/* Converts a raw byte array to base64 encoded C-string.
 * Returns a newly allocated buffer.
 * If bytes_len is 0, returns NULL */
char* base64(const unsigned char* bytes, size_t bytes_len);

#endif
