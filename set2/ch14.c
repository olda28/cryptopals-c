#include "ch14.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ch09.h"
#include "ch11.h"
#include "../set1/ch07.h"

static bytes_t key = NO_BYTES;
static bytes_t secret = NO_BYTES;
static bytes_t prefix = NO_BYTES;

bytes_t aes128_ecb_oracle_surround(const bytes_t in) {
    if (key.len == 0) key = random_bytes(16);
    if (prefix.len == 0) prefix = random_bytes(randint(3, 38));
    if (secret.len == 0) secret = unbase64("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK");
    bytes_t secret_in = {
        .bytes = malloc(prefix.len + in.len + secret.len),
        .len = prefix.len + in.len + secret.len
    };
    memcpy(secret_in.bytes, prefix.bytes, prefix.len);
    memcpy(secret_in.bytes + prefix.len, in.bytes, in.len);
    memcpy(secret_in.bytes + prefix.len + in.len, secret.bytes, secret.len);
    pkcs7_pad(&secret_in, 16);

    const bytes_t out = aes128_ecb(secret_in, key, true);
    free_bytes(&secret_in);
    return out;
}

void ch14_cleanup(void) {
    free_bytes(&key);
    free_bytes(&secret);
    free_bytes(&prefix);
}

/* Code copied over from ch12.c */
bytes_t aes128_ecb_crack_surround(bytes_t (*f)(bytes_t)) {
    // 1.1 Detect block size
    size_t extra_len = 0;
    int last_len = 0;
    int blocksize = 0;
    for (size_t i = 1; i < 34; i++) {
        bytes_t in = repeat_char('A', i);
        bytes_t enc = (*f)(in);
        free(enc.bytes); // keep length
        free_bytes(&in);

        if (last_len && enc.len - last_len > 1) {
            // big jump (dipped into next block)
            blocksize = enc.len - last_len; // how much it jumps by
            extra_len = last_len - i; // how many "our" bytes were needed to reach before-padding of last blocksize block
            break;
        }
        last_len = enc.len;
    }
    printf("blocksize: %d\n", blocksize);

    if (blocksize != 16) return NO_BYTES; // Support only AES128 at this time
    if (extra_len == 0) return NO_BYTES; // this really shouldn't be possible

    // 1.2. Find prefix length
    int prefix_len = -1;
    for (int p = 0; p < blocksize && prefix_len < 0; p++) {
        bytes_t payload = { .bytes = malloc(p + 32), .len = p + 32 };
        repeat_char_into(payload.bytes, 'P', p);
        repeat_char_into(payload.bytes + p, 'X', 32);

        bytes_t enc = (*f)(payload);
        free_bytes(&payload);
        for (size_t b = 0; b < enc.len / blocksize - 1; b++) {
            const unsigned char* block_a = enc.bytes + b * blocksize;
            const unsigned char* block_b = enc.bytes + (b + 1) * blocksize;
            if (!memcmp(block_a, block_b, blocksize)) {
                prefix_len = b * blocksize - p;
                break;
            }
        }
        free_bytes(&enc);
    }

    if (prefix_len < 0) return NO_BYTES;
    // 2. Detect ECB
    const bool ecb = is_aes128_ecb(f, blocksize);
    if (!ecb) return NO_BYTES;

    // 3. Attack!
    unsigned char** dict = malloc(96 * sizeof(unsigned char*));
    for (int i = 0; i < 96; i++) { dict[i] = malloc(blocksize * sizeof(unsigned char)); }

    const int secret_len = extra_len - prefix_len;
    const int secret_block_len = (int)ceil((double)secret_len / blocksize) * blocksize;
    const int prepad_len = (blocksize - prefix_len % blocksize) % blocksize;

    bytes_t cracked = { .bytes = malloc(secret_len), .len = 0 };

    for (int n = 1; n <= secret_len; n++) {
        const size_t payload_len = secret_block_len - n;
        const size_t input_len = prepad_len + payload_len + cracked.len + 1;
        const size_t target_offset = prefix_len + input_len - blocksize;
        // 3.1. Populate dictionary
        // [PREFIX][prepad]|[payload][cracked][i]|[SECRET.....]
        for (int i = 0; i < 96; i++) {
            bytes_t in = { .bytes = malloc(input_len), .len = input_len};
            repeat_char_into(in.bytes, 'P', prepad_len);
            repeat_char_into(in.bytes + prepad_len, 'A', payload_len);
            memcpy(in.bytes + prepad_len + payload_len, cracked.bytes, cracked.len);

            in.bytes[in.len - 1] = i == 0 ? '\n' : i + 0x1F;

            bytes_t enc = (*f)(in);
            memcpy(dict[i], enc.bytes + target_offset, blocksize);

            free_bytes(&in);
            free_bytes(&enc);
        }

        // 3.2. Get actual secret byte
        // [PREFIX][prepad]|[payload][cracked][S|ECRET.....]

        // only fill in prepad and payload, the rest will be shifted into place by the SECRET coming in
        bytes_t in = { .bytes = malloc(prepad_len + payload_len), .len = prepad_len + payload_len};
        repeat_char_into(in.bytes, 'P', prepad_len);
        repeat_char_into(in.bytes + prepad_len, 'A', payload_len);
        bytes_t enc = (*f)(in);

        // 3.3. Compare and find
        bool found = false;
        for (int i = 0; i < 96; i++) {
            found = !memcmp(dict[i], enc.bytes + target_offset, blocksize);
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
