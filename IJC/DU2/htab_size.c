#include "htab_private.h"

/**
 * @return number of items in map
*/
size_t htab_size(const htab_t * t){
    return t->size;
}