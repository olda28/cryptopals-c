#ifndef CH01_H
#define CH01_H
#include <string.h>

typedef struct {
    unsigned char* bytes;
    size_t len;
} bytes_t;

#define NO_BYTES ((bytes_t){ NULL, 0 })

char* hex(const unsigned char* bytes, size_t bytes_len);
bytes_t unhex(const char* hex);

char* base64(const unsigned char* bytes, size_t bytes_len);
bytes_t unbase64(const char* base);


#endif
