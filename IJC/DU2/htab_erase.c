#include "htab_private.h"

// bool htab_erase(htab_t * t, htab_key_t key){
//     size_t index = htab_hash_function(key) % t->arr_size;

//     for( htab_item_t * item = t->arr_ptr[index]; item != NULL; item = item->next  ){
//         if( !strcmp( item->pair.key, key ) ){
//             htab_item_t * forErase = item;
//             item = forErase->next;
//             htab_free_item(forErase);
//             t->size--;
//             return true;
//         }
//     }


//     return false;
// }

bool htab_erase(htab_t * t, htab_key_t key){
    size_t index = htab_hash_function(key) % t->arr_size;

    htab_item_t * current = t->arr_ptr[index];
    htab_item_t * previous = NULL;

    while( current != NULL ){
        if( strcmp(current->pair.key,key) == 0 ){
            if( current == t->arr_ptr[index] ){
                t->arr_ptr[index] = t->arr_ptr[index]->next;
            }
            else{
                previous->next = current->next;
            }
            htab_free_item(current);
            t->size--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}