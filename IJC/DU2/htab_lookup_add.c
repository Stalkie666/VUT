#include "htab_private.h"

/**
 * @return pointer to pair if success and NULL if fail
*/
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){
    // get index
    size_t index = htab_hash_function(key) % t->arr_size;
    // get pointer for first item in list
    htab_item_t ** tmp = &t->arr_ptr[index];
    // until pointer is not null
    while(  *tmp != NULL ){
        // chack if pair.key is equal to given key
        if( strcmp( (*tmp)->pair.key,key ) == 0 ){
            return &(*tmp)->pair;
        }
        // change pointer on next item in list
        tmp = &(*tmp)->next;
    }

    // if item was not found, make new item and check if alloc success
    *tmp = htab_init_item(key);
    if( *tmp == NULL ){
        return NULL;
    }
    //increase umber of words
    t->size++;
    return &(*tmp)->pair;
}