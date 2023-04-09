#include "htab_private.h"

void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
    for( size_t i = 0; i < t->arr_size; ++i){
        for(    htab_item_t * ptr = t->arr_ptr[i]; 
                ptr != NULL; 
                ptr = ptr->next ){
            f(&ptr->pair);
        }
    }
}