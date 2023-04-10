#include <stdlib.h>
#include "htab_private.h"

void htab_free(htab_t * t){
    htab_clear(t);
    free(t->arr_ptr);
    free(t);
}