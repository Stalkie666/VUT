#include <stdlib.h>
#include "htab_private.h"

void htab_free_item(htab_item_t * item){
    //mozny segfault - jeste uvidim jak to dodelam
    free( (char*)item->pair.key );
    free(item);
}