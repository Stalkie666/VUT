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