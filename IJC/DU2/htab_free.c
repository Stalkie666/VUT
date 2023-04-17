// htab_free.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdlib.h>
#include "htab_private.h"
/**
 * free memory of map
*/
void htab_free(htab_t * t){
    // clean whole array
    htab_clear(t);
    // free array of pointers
    free(t->arr_ptr);
    // free structure
    free(t);
}