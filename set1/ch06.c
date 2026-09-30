#include "ch06.h"

#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ch01.h"
#include "ch03.h"
#include "ch05.h"

static int bitcount(const uint8_t x) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        count += (x >> i) & 1;
    }
    return count;
}

static int hamming(const unsigned char* a, size_t a_len, const unsigned char* b, size_t b_len) {
    if (a_len != b_len)
        return -1;

    int distance = 0;
    for (size_t i = 0; i < a_len; i++) {
        const unsigned char xor = a[i] ^ b[i];
        distance += bitcount(xor);
    }
    return distance;
}

static int find_keylen(const unsigned char* in, int min, int max) {
    int keylen = min;

    double best_kd = DBL_MAX;
    int best_keylen = 0;
    while (keylen < max) {
        const int kd_12 = hamming(in, keylen, in + keylen, keylen);
        const int kd_23 = hamming(in + keylen, keylen, in + 2*keylen, keylen);
        const int kd_34 = hamming(in + 2*keylen, keylen, in + 3*keylen, keylen);
        const int kd_14 = hamming(in, keylen, in + 3*keylen, keylen);
        const int kd_13 = hamming(in, keylen, in + 2*keylen, keylen);
        const int kd_24 = hamming(in + keylen, keylen, in + 3*keylen, keylen);
        const double kd = (kd_12 + kd_23 + kd_34 + kd_14 + kd_13 + kd_24)/6.0;
        const double kd_normal = (double)kd / keylen;
        if (kd_normal < best_kd) {
            best_kd = kd_normal;
            best_keylen = keylen;
        }
        printf("keysize: %d, distance: %f\n", keylen, kd_normal);
        keylen++;
    }
    return best_keylen;
}

repeat_crack_result_t repeat_xor_crack(unsigned char* in, size_t in_len) {
    const int MAX_KEYLEN = (int)fmin(40, (double)in_len / 4); // we need at least four keysize blocks
    const int keylen = find_keylen(in, 2, MAX_KEYLEN);

    bytes_t* columns = malloc(keylen * sizeof(bytes_t));
    for (int i = 0; i < keylen; i++) {
        columns[i] = (bytes_t){ .bytes = malloc(in_len / keylen + 1), .len = 0 };
    }

    // Transpose blocks
    for (int i = 0; (size_t)i < in_len; i++) {
        const int j = i % keylen;
        bytes_t* col = &columns[j];
        col->bytes[col->len++] = in[i];
    }

    // Attempt Single-XOR crack
    unsigned char* key = malloc(keylen);
    for (int i = 0; i < keylen; i++) {
        const bytes_t* col = &columns[i];
        const crack_result_t cracked = single_xor_crack(col->bytes, col->len);
        if (!cracked.bytes)
            key[i] = '?';
        else
            key[i] = cracked.key;
        free(cracked.bytes);
        free(col->bytes);
    }
    free(columns);

    // Use the assembled key
    const bytes_t xor = repeat_xor(in, in_len, key, keylen);

    return (repeat_crack_result_t){
        .bytes = xor.bytes,
        .len = xor.len,
        .key = key,
        .key_len = keylen
    };
}
