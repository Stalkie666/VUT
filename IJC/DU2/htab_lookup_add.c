#include "htab_private.h"

htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){
    size_t index = htab_hash_function(key) % t->arr_size;

    htab_item_t ** tmp = &t->arr_ptr[index];
    while(  *tmp != NULL ){
        if( strcmp( (*tmp)->pair.key,key ) == 0 ){
            return &(*tmp)->pair;
        }
        tmp = &(*tmp)->next;
    }

    *tmp = htab_init_item(key);
    if( *tmp == NULL ){
        return NULL;
    }
    t->size++;
    return &(*tmp)->pair;
}