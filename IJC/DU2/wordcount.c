#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "io.h"

#define HTAB_SIZE 256
#define MAX_WORD_LEN 256

void printGivenPair(htab_pair_t *mi){
    printf("%s\t%d\n",mi->key,mi->value);
}

int main(){
    htab_t *m = htab_init(HTAB_SIZE);
    if(m == NULL){
        fprintf(stderr,"Allocation of memory failed.\n");
        return EXIT_FAILURE;
    }

    bool longWarned = false;
    int len;
    char word[MAX_WORD_LEN];

    while( (len = read_word(word,MAX_WORD_LEN,stdin)) != EOF ){
        if( (len >= MAX_WORD_LEN - 1) && !longWarned ){
            fprintf( stderr, "Word is longer than %d and was shorten.\n", MAX_WORD_LEN );
            longWarned = true;
        }

        htab_pair_t *pair =htab_lookup_add(m,word);
        if(pair == NULL){
            fprintf(stderr,"Allocation of memory failed.\n");
            htab_free(m);
            return EXIT_FAILURE;
        }
        pair->value++;
    }

    htab_for_each(m,printGivenPair);

    htab_free(m);

    return EXIT_SUCCESS;
}
