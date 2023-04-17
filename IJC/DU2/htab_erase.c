#include "htab_private.h"

/**
 * @return true if erase of pair was successfull, false if pair was not found
*/
bool htab_erase(htab_t * t, htab_key_t key){
    //get index for pair
    size_t index = htab_hash_function(key) % t->arr_size;

    // make temporaly variables and init it
    htab_item_t * current = t->arr_ptr[index];
    htab_item_t * previous = NULL;

    // loking for correct pair
    while( current != NULL ){
        // check if pair is correct
        if( strcmp(current->pair.key,key) == 0 ){
            // first element in list
            if( current == t->arr_ptr[index] ){
                t->arr_ptr[index] = t->arr_ptr[index]->next;
            }
            // other elements
            else{
                previous->next = current->next;
            }
            // free item
            htab_free_item(current);
            t->size--;
            return true;
        }
        // change current lelement of list
        previous = current;
        current = current->next;
    }
    return false;
}