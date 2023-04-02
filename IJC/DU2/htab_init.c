#include <stdlib.h>
#include "htab_private.h"

htab_t *htab_init(const size_t n){
    htab_t * retVal = malloc( sizeof(htab_t) );
    if( retVal == NULL ){
        return NULL;
    }
    retVal->arr_ptr = malloc(n * sizeof(htab_item_t*));
    if( retVal->arr_ptr == NULL ){
        free(retVal);
        return NULL;
    }
    retVal->size = 0;
    retVal->arr_size = n;
    for(int i = 0; i < n; ++i){
        retVal->arr_ptr[i] = NULL;
    }
    return retVal;
}