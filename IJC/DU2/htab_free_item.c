#include <stdlib.h>
#include "htab_private.h"

void htab_free_item(htab_item_t * item){
    free( (char*)item->pair.key );
    free(item);
}