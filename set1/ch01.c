#include "ch01.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void free_bytes(bytes_t* f) {
    free(f->bytes);
    f->bytes = NULL;
    f->len = 0;
}

/** @return Integer value of an ASCII hex byte */
static int deascii_hex(const char c) {
    const char lower = (char)tolower(c);
    if (lower >= '0' && lower <= '9')
        return c - '0';
    if (lower >= 'a' && lower <= 'f')
        return c - 'a' + 10;
    return -1;
}

/** @return ASCII character in hex of a raw byte */
static char ascii_hex(const uint8_t c) {
    if (c <= 9)
        return (char)(c + '0');
    if (c <= 15)
        return (char)(c - 10 + 'a');
    return 0x0;
}

char* hex(const unsigned char* bytes, size_t bytes_len) {
    if (bytes_len == 0) return NULL;
    const size_t hex_len = bytes_len * 2;

    char* buf = malloc(hex_len + 1);

    for (size_t i = 0; i < bytes_len; i++) {
        const char msb = ascii_hex(bytes[i] >> 4);
        const char lsb = ascii_hex(bytes[i] & 0x0F);
        buf[i * 2] = msb;
        buf[i * 2 + 1] = lsb;
    }
    buf[hex_len] = '\0';
    return buf;
}

bytes_t unhex(const char* hex) {
    const size_t hex_len = strlen(hex);
    if (hex_len % 2 != 0 || hex_len == 0)
        return NO_BYTES;

    const size_t buf_len = hex_len / 2;
    unsigned char* buf = malloc(buf_len);

    for (size_t i = 0; i < buf_len; i++) {
        const int msb = deascii_hex(hex[i * 2]);
        const int lsb = deascii_hex(hex[i * 2 + 1]);
        if (msb < 0 || lsb < 0) {
            free(buf);
            return NO_BYTES;
        }

        buf[i] = msb * 16 + lsb;
    }

    return (bytes_t)
    {
        .bytes = buf,
        .len = buf_len
    };
}


/* Base64
===============================
for (input) bytes 1, 2, 3
start with carry = 0

Notice that for byte 1, there is no carry [0 << 6 => 0]
Notice that for byte 4, there is no byte [carry << 0 | 0 >> 6 => carry] [0 & (255 >> 2) => 0]
After every four blocks, carry is resetted back to 0 (see line above)

out = carry << 6 | byte[0] >> 2
carry = byte[0] & (255 >> 6)

out = carry << 4 | byte[1] >> 4
carry = byte[1] & (255 >> 4)

out = carry << 2 | byte[2] >> 6
carry = byte[2] & (255 >> 2)

out = carry << 0 | 0 >> 8
carry = 0 & (255 >> 2)

================================ */
static const char base64_alpha[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/** @return Base64 encoded C-string
 *  @attention Returns a newly allocated buffer
 */
char* base64(const unsigned char* bytes, size_t bytes_len) {
    if (bytes_len == 0)
        return NULL;
    const size_t base_len = (bytes_len + 2) / 3 * 4; // == ceil(bytes_len / 3) * 4
    unsigned char* decoded = malloc(base_len);

    const unsigned char* r = bytes;
    unsigned char* w = decoded;

    uint8_t carry = 0;
    const size_t extra_pass = (bytes_len - 1) / 3; // extra pass for each carry-only at end of input triplet
    for (size_t i = 0; i <= bytes_len + extra_pass; i++) {
        const uint8_t j = i % 4; // 0, 1, 2, 3

        const uint8_t carry_mask_shift = 6 - 2 * j; // 6, 4, 2, 0
        const uint8_t byte_shift = 8 - carry_mask_shift; // 2, 4, 6, 8

        const uint8_t byte = j == 3 ? 0 : *r++; // last byte is just carry (byte = 0)
        *w++ = (carry << carry_mask_shift) | byte >> byte_shift;
        carry = byte & (255 >> carry_mask_shift);
    }

    const size_t unpadded_len = (size_t)(w - decoded);
    const size_t padding_len = base_len - unpadded_len;

    char* base = malloc(base_len + 1);

    // Translate to base64 alphabet
    for (size_t i = 0; i < unpadded_len; i++) {
        base[i] = base64_alpha[decoded[i]];
    }

    // Append padding
    for (size_t i = 0; i < padding_len; i++) {
        base[unpadded_len + i] = '=';
    }
    base[base_len] = '\0';

    free(decoded);
    return base;
}

/* UnBase64
 * for (output) bytes 1, 2, 3
 * ===========================================
 * The 4th write (fourth ascii byte) is skipped.
 *
 * out = (byte[0] & 0xff) << 2 | byte[1] >> 4
 * out = (byte[1] & 0x0f) << 4 | byte[2] >> 2
 * out = (byte[2] & 0x03) << 6 | byte[3] >> 0 */

bytes_t unbase64(const char* base) {
    if (!base || *base == '\0') return NO_BYTES;
    const size_t base_len = strlen(base);
    bytes_t out = {
        .bytes = malloc(base_len),
        .len = 0
    };

    const char* r = base;
    unsigned char* w = out.bytes;
    size_t skip_count = 0;
    for (size_t i = 0; i < base_len - 1 - skip_count; i++) {
        if (*(r + 1) == '=') break;
        const unsigned char byte = strchr(base64_alpha, *r) - base64_alpha;
        const unsigned char next_byte = strchr(base64_alpha, *++r) - base64_alpha;

        const uint8_t j = i % 3; // 0, 1, 2
        const uint8_t bitmask_shift = j * 2; // 0, 2, 4
        const uint8_t bitmask = 0xff >> bitmask_shift;
        const uint8_t next_byte_shift = 4 - bitmask_shift; // 4, 2, 0
        const uint8_t byte_shift = (j + 1) * 2; // 2, 4, 6

        const unsigned char byte_msb = (byte & bitmask) << byte_shift;
        const unsigned char byte_lsb = next_byte >> next_byte_shift;
        *w++ = byte_msb | byte_lsb;

        if (j == 2) {
            r++;
            skip_count++;
        }
    }
    out.len = w - out.bytes;

    return out;
}

/* Falls back to hex if nonprintable bytes are found */
static const char try_ascii_fail_prefix[] = "(hex) ";
static const size_t try_ascii_fail_len = strlen(try_ascii_fail_prefix);

char* try_ascii(const unsigned char* in, size_t in_len) {
    if (in_len == 0) return NULL;
    char* ascii = malloc(in_len + 1);
    for (size_t i = 0; i < in_len; i++) {
        if (isprint(in[i]) || isspace(in[i])) ascii[i] = (char)in[i];
        else goto fail;
    }
    ascii[in_len] = '\0';
    return ascii;

fail:
    free(ascii);
    char* in_hex = hex(in, in_len);
    if (!in_hex) return NULL;
    ascii = malloc(try_ascii_fail_len + in_len * 2 + 1);
    memcpy(ascii, try_ascii_fail_prefix, try_ascii_fail_len); // copy (hex) prefix
    memcpy(ascii + try_ascii_fail_len, in_hex, in_len * 2); // copy hex
    ascii[try_ascii_fail_len + in_len * 2] = '\0'; // NUL byte
    free(in_hex); // free unused hex
    return ascii;
}

bytes_t decode(const char* in, ENCODING encoding) {
    switch (encoding) {
    case NONE:
        return (bytes_t){ .bytes = (unsigned char*)in, .len = strlen(in) };
    case BASE64:
        return unbase64(in);
    case HEX:
        return unhex(in);
    }
    return NO_BYTES;
}
