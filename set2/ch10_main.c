#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ch10.h"
#include "../set1/ch01.h"
#include "../set1/ch04.h"
#include "../set1/ch07.h"


int main(void) {
    const char filename[] = "./set2/ch10-data.txt";
    const bool piped = !isatty(STDOUT_FILENO);

    bytes_t key = { .bytes = malloc(16), .len = 16 };
    memcpy(key.bytes, "YELLOW SUBMARINE", 16);

    const unsigned char iv_arr[16] = { 0 };
    bytes_t iv = { .bytes = malloc(16), .len = 16 };
    memcpy(iv.bytes, iv_arr, 16);

    FILE* file = fopen(filename, "r");
    file_bytes_t f = read_file(file, NEWLINE_STRIP, BASE64);
    decode_full(&f);

    bytes_t decrypted = aes128_cbc(f.decoded, key, iv, false);
    char* decrypted_ascii = try_ascii(decrypted);

    char* key_ascii = try_ascii(key);
    char* iv_ascii = try_ascii(iv);
    fprintf(stderr,
           "in: (%s)\n"
                 "key: %s\n"
                 "iv : %s\n"
                 "out: %s",
                 filename, key_ascii, iv_ascii, piped ? "(pipe)\n" : "");
    printf("%s", decrypted_ascii);


    fclose(file);
    close_file(&f);
    free_bytes(&key);
    free_bytes(&iv);
    free_bytes(&decrypted);
    free(key_ascii);
    free(iv_ascii);
    free(decrypted_ascii);
}
