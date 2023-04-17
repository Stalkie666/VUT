// htab_private.h
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#ifndef HTAB_PRIVATE_H__
#define HTAB_PRIVATE_H__


#include "htab.h"

// private structure of item
typedef struct htab_item{
    htab_pair_t pair;       // pair of key and value
    struct htab_item *next; // pointer to the next item
}htab_item_t;

// private structure made by instructions in assignment
struct htab{
    size_t size;            // actual number of words in map
    size_t arr_size;        // size of pointer arrays
    htab_item_t ** arr_ptr; // array of pointers
};

// init item for word
htab_item_t * htab_init_item(htab_key_t key);

// free item for word
void htab_free_item(htab_item_t * item);

#endif