#include "ch01.h"
#include "ch02.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char* fixed_xor_raw(const unsigned char* a, size_t a_len, const unsigned char* b, size_t b_len){
    if (a_len != b_len)
        return NULL;

   unsigned char* out = malloc(a_len);
   for (size_t i = 0; i < a_len; i++){
        out[i] = a[i] ^ b[i];
   }
   return out;
}

char* fixed_xor_hex(const char* a_hex, const char* b_hex){
    const size_t a_len = strlen(a_hex);
    const size_t b_len = strlen(b_hex);
    if (a_len != b_len)
        return NULL;

   const bytes_t a = unhex(a_hex);
   if (a.bytes == NULL)
       return NULL;

   const bytes_t b = unhex(b_hex);
   if (b.bytes == NULL){
       free(a.bytes);
       return NULL;
   }

   unsigned char* out = fixed_xor_raw(a.bytes, a.len, b.bytes, b.len);
   free(a.bytes);
   free(b.bytes);
   if (out == NULL)
       return NULL;

   char* out_hex = malloc(a.len+1);
   for (size_t i = 0; i < a.len; i++){
       snprintf(out_hex+i*2, 3, "%02x", out[i]);
   }
   out_hex[a.len] = '\0';
   free(out);

   return out_hex;
}
