#include <stdlib.h>
#include "htab_private.h"

htab_item_t * htab_init_item(htab_key_t key){
    htab_item_t * retVal = malloc(sizeof(htab_item_t));
    if(!retVal)
        return NULL;

    int allocLen = strlen(key) + 1;
    retVal->pair.key = malloc(allocLen * sizeof(char));
    if( !(retVal->pair.key) ){
        free(retVal);
        return NULL;
    }
    strcpy((char*)retVal->pair.key, key);
    //retVal->pair.key = key;

    retVal->pair.value = 0;
    retVal->next = NULL;
    return retVal;
}