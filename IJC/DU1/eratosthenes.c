// erathosthenes.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "bitset.h"

#define N_PRIME_NUM_TO_PRINT 10

//projit jednotliva cisla v poli po bitech, pouzit bitovy posuv
    //filter bude unsigned long stejne jako pole.array[] 
    // asi nejak takhle: ,filter=1; porovnam; filter << 1 - bit posuv; a znova
    //jeste nekde ukladat se kterym cislem to porovnavam
    //asi proste budu odecitat, nejjedodussi

//po dokonceni nulovani cisel co nejsou prvocisla, 
    //vytisknout 10 poslednich(?pomoci rekurze asi,ale slo by to i jinak)


void Eratosthenes( bitset_t pole ){
        for(bitset_index_t i = 1; i < ( pole[0] / (sizeof(bitset_index_t)*8) + 2 ); ++i ){
            pole[i] = ULONG_MAX;
        }
        bitset_index_t tmp;
        for( bitset_index_t i = 2; i < pole[0]; ++i ){
            if( bitset_getbit(pole,i) == 1 ){
                for(bitset_index_t j = i + 1; i < pole[0]; ++i){
                    if( bitset_getbit(pole,j) % i == 0 ){
                        bitset_setbit(pole,j,0);
                    }
                }
            }
        }

        bitset_index_t primeNumbers[N_PRIME_NUM_TO_PRINT] = {0};

        bitset_index_t index = N_PRIME_NUM_TO_PRINT;

        bitset_index_t backwards = pole[0];

        while(index && backwards > 0){
            if( bitset_getbit(pole,backwards) ){
                primeNumbers[index-1] = backwards;
                index--;
            }
            backwards--;
        }

        for(bitset_index_t i = 0; i < N_PRIME_NUM_TO_PRINT; ++i){
            if(primeNumbers[i] != 0){
                printf("%d\n",primeNumbers[i]);
            }
        }
}