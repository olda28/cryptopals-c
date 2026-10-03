#ifndef CH01_H
#define CH01_H
#include <string.h>

typedef enum ENCODING {
    NONE,
    BASE64,
    HEX
} ENCODING;

typedef struct {
    unsigned char* bytes;
    size_t len;
} bytes_t;
#define NO_BYTES ((bytes_t){ NULL, 0 })
void free_bytes(bytes_t* f);

typedef struct {
    unsigned char* bytes;
    size_t len;
    int line;
} line_bytes_t;
#define NO_LINE_BYTES ((line_bytes_t){ NULL, 0, -1 })
void free_line_bytes(line_bytes_t* f);

char* hex(const unsigned char* bytes, size_t bytes_len);
bytes_t unhex(const char* hex);

char* base64(const unsigned char* bytes, size_t bytes_len);
bytes_t unbase64(const char* base);

char* try_ascii(const unsigned char* in, size_t in_len);
bytes_t decode(const char* in, ENCODING encoding);


#endif
