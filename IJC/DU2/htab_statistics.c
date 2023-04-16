#include <limits.h>
#include <stdio.h>
#include "htab_private.h"

void htab_statistics(const htab_t * t){
    if(t == NULL){
        fprintf(stderr,"Ukazatel nikam neukazuje (NULL), neni mozna statistika.\n");
        return;
    }
    int min = INT_MAX;
    int max = 0;
    int ptrWithContent = 0;

    for( size_t i = 0; i < t->arr_size; ++i){
        int length = 0;
        for(    htab_item_t * ptr = t->arr_ptr[i]; 
                ptr != NULL; 
                ptr = ptr->next ){
                    ++length;
        }
        if( min > length ) min = length;
        if( max < length ) max = length;
        if( t->arr_ptr[i] != NULL ) ++ptrWithContent;
    }

    double avg = t->size / (double)ptrWithContent;

    printf("Max: %d\n",max);
    printf("Min: %d\n",min);
    printf("Average: %lf\n",avg);
}