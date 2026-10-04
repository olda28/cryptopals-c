#include "ch12.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ch09.h"
#include "ch11.h"
#include "../set1/ch07.h"

static bytes_t key = NO_BYTES;
static bytes_t secret = NO_BYTES;

bytes_t aes128_ecb_oracle_append(const bytes_t in) {
    if (key.len == 0) key = random_bytes(16);
    if (secret.len == 0) secret = unbase64("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK");
    bytes_t secret_in = {
        .bytes = malloc(in.len + secret.len),
        .len = in.len + secret.len
    };
    memcpy(secret_in.bytes, in.bytes, in.len);
    memcpy(secret_in.bytes + in.len, secret.bytes, secret.len);
    pkcs7_pad(&secret_in, 16);

    const bytes_t out = aes128_ecb(secret_in, key, true);
    free_bytes(&secret_in);
    return out;
}

void ch12_cleanup(void) {
    free_bytes(&key);
    free_bytes(&secret);
}

bytes_t aes128_ecb_crack_append(bytes_t (*f)(bytes_t)) {
    // 1. Detect block size (and secret length)
    size_t secret_max_len = 0;
    int blocksize = 0;
    for (size_t i = 1; i < 34; i++) {
        bytes_t in = repeat_char('A', i);
        bytes_t enc = (*f)(in);
        free(enc.bytes); // keep length
        free_bytes(&in);

        if (blocksize && enc.len - blocksize > 1) { // big jump
            secret_max_len = blocksize;
            blocksize = enc.len - blocksize;
            break;
        }
        blocksize = enc.len;
    }
    printf("blocksize: %d\n", blocksize);
    if (blocksize != 16) return NO_BYTES; // Support only AES128 at this time
    if (secret_max_len == 0) return NO_BYTES; // this really shouldn't be possible

    // 2. Detect ECB
    const bool ecb = is_aes128_ecb(f, blocksize);
    if (!ecb) return NO_BYTES;

    // 3. Attack!
    bytes_t cracked = {
        .bytes = malloc(secret_max_len),
        .len = 0
    };
    unsigned char** dict = malloc(96 * sizeof(unsigned char*));
    for (int i = 0; i < 96; i++) { dict[i] = malloc(blocksize * sizeof(unsigned char)); }
    const int end = (int)ceil((double)secret_max_len / blocksize)*blocksize;

    for (size_t n = 1; n < secret_max_len; n++) {
        // 3.1. Populate dictionary
        for (int i = 0; i < 96; i++) {
            bytes_t in = { .bytes = malloc(end), .len = end };
            bytes_t stub = repeat_char('A', end-n);
            memcpy(in.bytes, stub.bytes, stub.len);
            memcpy(in.bytes+stub.len, cracked.bytes, cracked.len);
            free_bytes(&stub);

            in.bytes[end-1] = i == 0 ? '\n' : i + 0x1F;
            bytes_t enc = (*f)(in);

            memcpy(dict[i], enc.bytes + end - blocksize, blocksize);
            free_bytes(&in);
            free_bytes(&enc);
        }

        // 3.2. Get actual secret byte
        bytes_t in = repeat_char('A', end-n);
        bytes_t enc = (*f)(in);

        // 3.3. Compare and find
        bool found = false;
        for (int i = 0; i < 94; i++) {
            found = !memcmp(dict[i], enc.bytes + end - blocksize, blocksize);
            if (found) {
                cracked.bytes[cracked.len++] = i == 0 ? '\n' : i + 0x1F;
                break;
            }
        }
        free_bytes(&in);
        free_bytes(&enc);

        if (!found) break; // We're done!
    }
    for (int i = 0; i < 96; i++) { free(dict[i]); }
    free(dict);

    return cracked;
}
