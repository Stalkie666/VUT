// htab_hash_function.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdint.h>
#include "htab_private.h"

/**
 * this function was given in the assignment and was just copied
 * @return number calculated in given function
*/
size_t htab_hash_function(htab_key_t str){
    uint32_t h = 0;
    const unsigned char *p;
    for( p=(const unsigned char*)str; *p != '\0'; ++p )
        h = 65599 * h + *p;
    return h;
}