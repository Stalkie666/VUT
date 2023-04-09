#include "htab_private.h"

bool htab_erase(htab_t * t, htab_key_t key){
    size_t index = htab_hash_function(key);

    for( htab_item_t * item = t->arr_ptr[index]; item != NULL; item = item->next  ){
        if( !strcmp( item->pair.key, key ) ){
            htab_item_t * forErase = item;
            item = forErase->next;
            htab_free_item(forErase);
            t->size--;
            return true;
        }
    }


    return false;
}