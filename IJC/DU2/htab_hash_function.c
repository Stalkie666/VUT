#include <stdint.h>
#include "htab_private.h"

/**
 * this function was given in the assignment and was jast copied
 * @return number calculated in given function
*/
size_t htab_hash_function(htab_key_t str){
    uint32_t h = 0;
    const unsigned char *p;
    for( p=(const unsigned char*)str; *p != '\0'; ++p )
        h = 65599 * h + *p;
    return h;
}