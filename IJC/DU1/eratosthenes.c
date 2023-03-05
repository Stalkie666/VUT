// erathosthenes.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bitset.h"
#include "eratosthenes.h"

#define N_PRIME_NUM_TO_PRINT 10

void Eratosthenes( bitset_t pole ){
    //eratostenovo sito
    bitset_index_t maxNum = sqrt(pole[0]);
    for(bitset_index_t i = 2; i <= maxNum; ++i){
        //overeni validnosti cisla - bitu
        if(!( bitset_getbit(pole,i))){
            //nastaveni vsech nasobku cisla
            for(bitset_index_t i = 2; i*j <= pole[0]; j++){
                bitset_setbit(pole,i*j,1);
            }
        }
    }
}

// //pole pro poslednich N prvocisel
        // bitset_index_t primeNumbers[N_PRIME_NUM_TO_PRINT] = {0};
        // bitset_index_t index = N_PRIME_NUM_TO_PRINT;
        // bitset_index_t backwards = pole[0];

        // while(index && backwards > 0){
        //     if( !(bitset_getbit(pole,backwards)) ){
        //         primeNumbers[index-1] = backwards;
        //         index--;
        //     }
        //     backwards--;
        // }

        // for(bitset_index_t i = 0; i < N_PRIME_NUM_TO_PRINT; ++i){
        //     if(primeNumbers[i] != 0){
        //         printf("%d\n",primeNumbers[i]);
        //     }
        // }