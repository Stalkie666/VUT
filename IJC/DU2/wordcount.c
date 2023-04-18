// wordcount.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "io.h"

#define HTAB_SIZE 256
#define MAX_WORD_LEN 256

/**
 * print key and value for given Pair
*/
void printGivenPair(htab_pair_t *mi){
    printf("%s\t%d\n",mi->key,mi->value);
}

int main(){
    // init map and check it
    htab_t *m = htab_init(HTAB_SIZE);
    if(m == NULL){
        fprintf(stderr,"Allocation of memory failed.\n");
        return EXIT_FAILURE;
    }

    // checker for first longer word
    bool isLonger = false;
    // length of word which was loaded
    int realLen;
    // word buffer
    char word[MAX_WORD_LEN];
    // read words until EOF
    while( (realLen = read_word(word,MAX_WORD_LEN,stdin)) != EOF ){
        // check length and if it has been already longer word
        if( (realLen >= MAX_WORD_LEN - 1) && !isLonger ){
            isLonger = true;
            fprintf( stderr, "Word is longer than %d and was shorten.\n", MAX_WORD_LEN );
        }
        // get pointer on pair from map by given word and check it if success
        htab_pair_t *pair = htab_lookup_add(m,word);
        if(pair == NULL){
            fprintf(stderr,"Allocation of memory failed.\n");
            htab_free(m);
            return EXIT_FAILURE;
        }
        pair->value++;
    }
    // print all pair of keys and their values
    htab_for_each(m,printGivenPair);

    //print some statistics
    #ifdef STATISTICS
        htab_statistics(m);
    #endif // STATISTICS

    // free unordered map
    htab_free(m);

    return EXIT_SUCCESS;
}
