#include "htab_private.h"
/**
 * use choosen function for each pair in map
*/
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
    // for each pointer in ptr_array
    for( size_t i = 0; i < t->arr_size; ++i){
        //for each item in list
        for(    htab_item_t * ptr = t->arr_ptr[i]; 
                ptr != NULL; 
                ptr = ptr->next ){
                    //call function on pair
                    f(&ptr->pair);
        }
    }
}