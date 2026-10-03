#include "ch10.h"

#include <stdio.h>

#include "../set1/ch07.h"

#include <stdlib.h>

#include "../set1/ch02.h"

bytes_t aes128_cbc(bytes_t in, bytes_t key, bytes_t iv, bool encrypt) {
    if (key.len != 16 || iv.len != 16) return NO_BYTES;
    if (in.len % 16 != 0) return NO_BYTES; // Without padding for now

    int blocks_n = in.len / 16;

    bytes_t out = {
        .bytes = malloc(in.len),
        .len = in.len
    };

    bytes_t prev_block = iv;
    for (int i = 0; i < blocks_n; i++) {
        const bytes_t block = { .bytes = in.bytes + i*16, .len = 16 };
        bytes_t step1 = encrypt ? fixed_xor(block, prev_block) : aes128_ecb(block, key, false);
        bytes_t step2 = encrypt ? aes128_ecb(step1, key, true) : fixed_xor(step1, prev_block);
        memcpy(out.bytes+i*16, step2.bytes, 16);
        prev_block = encrypt ? (bytes_t){ .bytes = out.bytes+i*16, .len = 16} : block;

        free_bytes(&step1);
        free_bytes(&step2);
    }
    return out;
}


bytes_t aes128_cbc_encrypt(bytes_t in, bytes_t key, bytes_t iv) {
    if (key.len != 16 || iv.len != 16) return NO_BYTES;
    if (in.len % 16 != 0) return NO_BYTES; // Without padding for now

    int blocks_n = in.len / 16;

    bytes_t out = {
        .bytes = malloc(in.len),
        .len = in.len
    };

    bytes_t prev_block = iv;
    for (int i = 0; i < blocks_n; i++) {
        const bytes_t block = { .bytes = in.bytes + i*16, .len = 16 };
        bytes_t xor_block = fixed_xor(block, prev_block);
        bytes_t block_enc = aes128_ecb(xor_block, key, true);
        memcpy(out.bytes+i*16, block_enc.bytes, 16);
        prev_block = block_enc;

        free_bytes(&block_enc);
        free_bytes(&xor_block);;
    }
    return out;
}
