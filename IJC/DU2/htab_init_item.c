#include <stdlib.h>
#include "htab_private.h"

/**
 * @return pointer on new item and NULL if mallocs failed 
*/
htab_item_t * htab_init_item(htab_key_t key){
    // alloc item and check if success
    htab_item_t * retVal = malloc(sizeof(htab_item_t));
    if(!retVal)
        return NULL;

    // count len of given key
    int allocLen = strlen(key) + 1;
    // alloc memory for key and check if success
    retVal->pair.key = malloc(allocLen * sizeof(char));
    if( !(retVal->pair.key) ){
        free(retVal);
        return NULL;
    }
    // copy key in allocated memory
    strcpy((char*)retVal->pair.key, key);
    // set more values and return
    retVal->pair.value = 0;
    retVal->next = NULL;
    return retVal;
}