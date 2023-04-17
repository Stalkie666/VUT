// htab_bucket_count.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include "htab_private.h"

// return allocated size of array
size_t htab_bucket_count(const htab_t * t){
    return t->arr_size;
}