// htab_find.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include "htab_private.h"

/**
 * @return pointer for pair, if found and NULL if not
*/
htab_pair_t * htab_find(const htab_t * t, htab_key_t key){
    // get index from hash function
    size_t index = htab_hash_function(key) % t->arr_size;

    // go throught list until valid pair is found and return pointer for that key
    for( htab_item_t * item = t->arr_ptr[index]; item != NULL; item = item->next ){
        if( strcmp(item->pair.key, key) == 0 ){
            return &item->pair;
        }
    }
    //return NULL if not
    return NULL;
}