#include <stdio.h>
#include <stdlib.h>

#include "ch11.h"
#include "ch13.h"
/*
email=olda@gmail.com&uid=9&role=user
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*-->            *-->            *-->
A = blocks[0] = "email=olda@gmail"
B = blocks[1] = ".com&uid=9&role="

email=XXXXXXXXXXadminBBBBBBBBBBB&uid=9&role=user
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*-->            *-->            *-->
C = blocks[1] = "admin" + 11*0xB (padding)
X = A + B + C
*/


int main(void) {
    bytes_t cracked = {
        .bytes = malloc(48), // 3 blocks
        .len = 48
    };

    bytes_t a = profile_for("olda@gmail.com");
    memcpy(cracked.bytes, a.bytes, 16); // A
    memcpy(cracked.bytes+16, a.bytes+16, 16); // B
    free_bytes(&a);

    char* b_input = malloc(26);
    repeat_char_into((unsigned char*)b_input, 'X', 10);
    memcpy(b_input+10, "admin", 5);
    repeat_char_into((unsigned char*)b_input+15, 11, 11);

    bytes_t b = profile_for(b_input);
    memcpy(cracked.bytes+32, b.bytes+16, 16); // C
    free_bytes(&b);
    free(b_input);

    // We will see the raw 0xB padding bytes - we haven't implemented padding stripping yet
    // The solution is correct nonetheless
    char* decrypt = profile_decrypt(cracked);
    printf("%s\n", decrypt);
    free_bytes(&cracked);

    free(decrypt);
    free_key();
    return 0;
}