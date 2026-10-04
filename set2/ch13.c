#include "ch13.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ch09.h"
#include "ch11.h"
#include "../set1/ch07.h"

const char* kv_get(const kv_map_t* map, const char *key) {
    for (size_t i = 0; i < map->len; i++) {
        if (!strcmp(map->pairs[i].key, key)) return map->pairs[i].value;
    }
    return NULL;
}
kv_map_t kv_parse(const char* encoded) {
    const size_t encoded_len = strlen(encoded);
    kv_map_t out = { .len = 1 };

    for (size_t i = 0; i < encoded_len; i++) {
        if (encoded[i] == '&') out.len++;
    }

    out.pairs = (kv_pair_t*) malloc(out.len * sizeof(kv_pair_t));
    for (size_t i = 0; i < out.len; i++) {
        out.pairs[i] = (kv_pair_t){ .key = malloc(50), .value = malloc(50) };
        int key_idx = 0;
        int value_idx = 0;
        bool equals_consumed = false;

        char c;
        while ((c = *encoded++) != '&' && c != '\0' && key_idx <= 48 && value_idx <= 48) {
            if (c == '=')
                equals_consumed = true;
            else if (equals_consumed)
                out.pairs[i].value[value_idx++] = c;
            else
                out.pairs[i].key[key_idx++] = c;
        }
        out.pairs[i].key[key_idx] = '\0';
        out.pairs[i].value[value_idx] = '\0';

    }
    return out;
}
char* kv_encode(const kv_map_t* map) {
    char* encoded = malloc(100);
    char* w = encoded;
    for (size_t i = 0; i < map->len; i++) {
        const char* key = map->pairs[i].key;
        const char* value = map->pairs[i].value;
        const size_t key_len = strlen(key);
        const size_t value_len = strlen(value);

        memcpy(w, key, key_len);
        w += key_len;
        *w++ = '=';
        memcpy(w, value, value_len);
        w += value_len;
        if (i != map->len-1) *w++ = '&';
    }
    *w = '\0';
    return encoded;
}
void kv_free(kv_map_t* map){
    for (size_t i = 0; i < map->len; i++) {
        free(map->pairs[i].key);
        free(map->pairs[i].value);
    }
    free(map->pairs);
}

static bytes_t key = NO_BYTES;
static bytes_t profile_encrypt(kv_map_t* map) {
    if (key.len == 0) key = random_bytes(16);

    char* encoded = kv_encode(map);
    bytes_t in = {
        .bytes = (unsigned char*)encoded,
        .len = strlen(encoded)
    };
    pkcs7_pad(&in, 16); // free's encoded

    const bytes_t encrypted = aes128_ecb(in, key, true);
    free_bytes(&in);
    return encrypted;
}
static kv_map_t profile_make(const char* email){
    const size_t len = strlen(email);

    char* sanitized = malloc(len+1);
    char* w = sanitized;

    // email sanitize
    char c;
    while ((c = *email++) != '\0') {
        if (c == '&' || c == '=') continue;
        *w++ = c;
    }
    *w = '\0';

    // uid
    srand(time(NULL));
    char* uid = malloc(2);
    memcpy(uid, (char[]){ (char)(rand() % 10 + '0'), '\0' }, 2);

    // role
    char* role = malloc(5);
    memcpy(role, "user\0", 5);

    const kv_map_t out = { .pairs = malloc(3 * sizeof(kv_pair_t)), .len = 3 };
    out.pairs[0] = (kv_pair_t){ .key = malloc(6),  .value = sanitized };
    memcpy(out.pairs[0].key, "email\0", 6);
    out.pairs[1] = (kv_pair_t){ .key = malloc(4), .value = uid };
    memcpy(out.pairs[1].key, "uid\0", 4);
    out.pairs[2] = (kv_pair_t){ .key = malloc(5), .value = role };
    memcpy(out.pairs[2].key, "role\0", 5);

    return out;
}

bytes_t profile_for(const char* email) {
    kv_map_t profile = profile_make(email);
    bytes_t encrypted = profile_encrypt(&profile);
    kv_free(&profile);
    return encrypted;
}
char* profile_decrypt(const bytes_t encrypted) {
    bytes_t decrypted = aes128_ecb(encrypted, key, false);
    char* decrypted_str = append_nul(decrypted);
    free_bytes(&decrypted);
    return decrypted_str;
}


void free_key(void) {
    free_bytes(&key);
}