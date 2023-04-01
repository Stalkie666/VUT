#ifndef HTAB_PRIVATE_H__
#define HTAB_PRIVATE_H__


#include "htab.h"

typedef struct htab_item{
    htab_pair_t pair;
    struct htab_item *next;
}htab_item_t;

struct htab{
    int size;
    int arr_size;
    htab_item_t ** arr_ptr;
};

htab_item_t * htab_init_item();

void htab_free_item(htab_item_t * item);

#endif