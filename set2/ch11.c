#include "ch11.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ch09.h"
#include "ch10.h"
#include "../set1/ch07.h"

bytes_t repeat_char(unsigned char c, int len) {
    if (len <= 0) return NO_BYTES;
    bytes_t out = {
        .bytes = malloc(len),
        .len = len
    };
    for (int i = 0; i < len; i++) {
        out.bytes[i] = c;
    }
    return out;
}

void repeat_char_into(unsigned char* dest, unsigned char c, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = c;
    }
}

bytes_t random_bytes(size_t len) {
    const bytes_t out = {
        .bytes = malloc(len),
        .len = len
    };
    for (int i = 0; i < len; i++) {
        out.bytes[i] = (unsigned char)randint(0, 255);
    }
    return out;
}

static bool seeded = false;
int randint(int min, int max) {
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    return rand() % (max - min + 1) + min;
}

bytes_t aes128_random_mode(const bytes_t in) {
    srand(time(NULL));
    const bool cbc = rand() % 2; // 0 = ecb, 1 = cbc
    //printf("Encrypting with %s\n", cbc ? "CBC" : "ECB");
    bytes_t key = random_bytes(16);
    bytes_t iv = cbc ? random_bytes(16) : NO_BYTES;
    bytes_t prepend = random_bytes(randint(5, 10));
    bytes_t append = random_bytes(randint(5, 10));

    bytes_t plaintext = {
        .bytes = malloc(prepend.len + in.len + append.len),
        .len = prepend.len + in.len + append.len
    };
    memcpy(plaintext.bytes, prepend.bytes, prepend.len);
    memcpy(plaintext.bytes+prepend.len, in.bytes, in.len);
    memcpy(plaintext.bytes+prepend.len+in.len, append.bytes, append.len);
    pkcs7_pad(&plaintext, 16);

    const bytes_t encrypted = cbc ? aes128_cbc(plaintext, key, iv, 1) : aes128_ecb(plaintext, key, 1);
    free_bytes(&key);
    free_bytes(&iv);
    free_bytes(&prepend);
    free_bytes(&append);
    free_bytes(&plaintext);
    return encrypted;
}

bool is_aes128_ecb(bytes_t (*f)(bytes_t), int blocksize){
    bytes_t in = repeat_char('X', blocksize*3);
    bytes_t random = (*f)(in);
    bool result = false;
    for (int i = 0; i < random.len; i+=blocksize) {
        if (!result) result = !memcmp(random.bytes+i, random.bytes+i+blocksize, blocksize);
        else break;
    }
    free_bytes(&in);
    free_bytes(&random);
    return result;
}
