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