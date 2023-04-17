// htab_free_item.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdlib.h>
#include "htab_private.h"
/**
 * free memory  of given item
*/
void htab_free_item(htab_item_t * item){
    //free allocated key
    free( (char*)item->pair.key );
    // free item
    free(item);
}