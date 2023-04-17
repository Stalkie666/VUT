#include "htab_private.h"

// clean array of poiter
void htab_clear(htab_t * t){
    // just like for_each
    for( size_t i = 0; i < t->arr_size; ++i){
        // first item in list
        htab_item_t * ptr = t->arr_ptr[i];
        // freeing items
        while( ptr ){
            htab_item_t * tmp = ptr->next;
            htab_free_item(ptr);
            ptr = tmp;
        }
        // set first pointer in list to null
        t->arr_ptr[i] = NULL;
    }
    // set size of map to zero 
    t->size = 0;
}