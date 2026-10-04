#ifndef SET2_CH13_H
#define SET2_CH13_H
#include "../set1/ch01.h"
#include <stddef.h>

typedef struct {
    char* key;
    char* value;
} kv_pair_t;

typedef struct {
    kv_pair_t* pairs;
    size_t len;
} kv_map_t;

const char* kv_get(const kv_map_t* map, const char *key);
kv_map_t kv_parse(const char* encoded);
char* kv_encode(const kv_map_t* map);
void kv_free(kv_map_t* map);
// end == utility functions

bytes_t profile_for(const char* email); // encrypt
char* profile_decrypt(bytes_t encrypted); // decrypts

void free_key(void); // cleanup


#endif
