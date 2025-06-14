// htab_statistics.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <limits.h>
#include <stdio.h>
#include "htab_private.h"

/**
 * print statistic for max, min and average lists
*/
void htab_statistics(const htab_t * t){
    // check if given pointer is NULL
    if(t == NULL){
        fprintf(stderr,"Ukazatel nikam neukazuje (NULL), neni mozna statistika.\n");
        return;
    }
    // help variables
    int min = INT_MAX;
    int max = 0;
    int ptrWithContent = 0;

    // check all pointers in arrai
    for( size_t i = 0; i < t->arr_size; ++i){
        int length = 0;
        // check all items in given list 
        for(    htab_item_t * ptr = t->arr_ptr[i]; 
                ptr != NULL; 
                ptr = ptr->next ){
                    ++length;
        }

        
        // check if ptr is not NULL
        if( t->arr_ptr[i] != NULL ){
            // check if lenght is bigger or smaller
            if( min > length ) min = length;
            if( max < length ) max = length;
            ++ptrWithContent;
        } 
    }

    double avg = 0;
    // check if is any word in map
    if( ptrWithContent > 0 ) avg = t->size / (double)ptrWithContent;
    else{
        min = 0;
    }
    // print statistics
    fprintf(stderr,"Max: %d\n",max);
    fprintf(stderr,"Min: %d\n",min);
    fprintf(stderr,"Average: %lf\n",avg);
}