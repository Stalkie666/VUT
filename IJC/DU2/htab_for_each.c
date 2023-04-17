// htab_for_each.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

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