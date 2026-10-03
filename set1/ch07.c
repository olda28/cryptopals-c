#include "ch07.h"

#include <stdbool.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#include "ch01.h"

static bytes_t openssl_error(EVP_CIPHER_CTX *ctx, char* customError) {
    unsigned long errCode;
    while ((errCode = ERR_get_error())) {
        char* err = ERR_error_string(errCode, NULL);
        fprintf(stderr, "OpenSSL internal Error: %s\n", err);
    }
    if (customError)
        fprintf(stderr, "OpenSSL custom error: %s\n", customError);
    EVP_CIPHER_CTX_free(ctx);

    return NO_BYTES;
}

bytes_t aes128_ecb(bytes_t in, bytes_t key, bool encrypt) {
    if (in.len == 0 || key.len == 0) return NO_BYTES;
    if (in.len > INT_MAX || key.len > INT_MAX) return openssl_error(NULL, "Input/key buffer may not be larger than INT_MAX.");
    // Initialize context
    EVP_CIPHER_CTX* ctx;
    if (!((ctx = EVP_CIPHER_CTX_new()))) return openssl_error(ctx, NULL);
    // Initialize cipher and validate lengths
    if (!EVP_CipherInit_ex2(ctx, EVP_aes_128_ecb(), NULL, NULL, encrypt, NULL)) return openssl_error(ctx, NULL);
    if ((size_t)EVP_CIPHER_CTX_get_key_length(ctx) != key.len) return openssl_error(ctx, "Key must be 16 bytes.");
    if (!EVP_CIPHER_CTX_set_padding(ctx, 0)) return openssl_error(ctx, "Failed to disable padding");
    if (!EVP_CipherInit_ex2(ctx, NULL, key.bytes, NULL, encrypt, NULL)) return openssl_error(ctx, NULL);
    // Encrypt
    bytes_t out = {
        .bytes = malloc(in.len + 16),
        .len = 0
    };
    int update_len;
    int final_len;
    if (!EVP_CipherUpdate(ctx, out.bytes, &update_len, in.bytes, (int)in.len)) return openssl_error(ctx, NULL);
    if (!EVP_CipherFinal_ex(ctx, out.bytes+update_len, &final_len)) return openssl_error(ctx, NULL);
    EVP_CIPHER_CTX_free(ctx);

    out.len = (size_t)update_len + (size_t)final_len;
    return out;
}

