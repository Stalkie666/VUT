// htab_size.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include "htab_private.h"

/**
 * @return number of items in map
*/
size_t htab_size(const htab_t * t){
    return t->size;
}