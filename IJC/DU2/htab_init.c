// htab_init.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdlib.h>
#include "htab_private.h"
/**
 * @return pointer on unordered map if success and NULL if fail somewhere
*/
htab_t *htab_init(const size_t n){
    // check given value
    if(n == 0)
        return NULL;
    // alloc memory for map and check if success
    htab_t * retVal = malloc( sizeof(htab_t) );
    if( retVal == NULL ){
        return NULL;
    }
    // alloc memory for array of pointers for list of items and check success
    retVal->arr_ptr = malloc(n * sizeof(htab_item_t*));
    if( retVal->arr_ptr == NULL ){
        free(retVal);
        return NULL;
    }
    // set variables in structure and set pointers in array to NULL
    retVal->size = 0;
    retVal->arr_size = n;
    for(size_t i = 0; i < n; ++i){
        retVal->arr_ptr[i] = NULL;
    }
    return retVal;
}