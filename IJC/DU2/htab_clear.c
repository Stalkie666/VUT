#include "htab_private.h"

void htab_clear(htab_t * t){
    for( size_t i = 0; i < t->arr_size; ++i){
        htab_item_t * ptr = t->arr_ptr[i];
        while( ptr ){
            htab_item_t * tmp = ptr->next;
            htab_free_item(ptr);
            ptr = tmp;
        }
        t->arr_ptr[i] = NULL;
    }
    t->size = 0;
}