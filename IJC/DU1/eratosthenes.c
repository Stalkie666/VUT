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
    // cislo 0 a 1 nastavit na bit 1
    bitset_setbit(pole,0,1);
    bitset_setbit(pole,1,1);
    bitset_index_t maxNum = sqrt(pole[0]);
    for(bitset_index_t i = 2; i <= maxNum; ++i){
        //overeni validnosti cisla - bitu
        if(!( bitset_getbit(pole,i))){
            //nastaveni vsech nasobku cisla
            for(bitset_index_t j = 2; i*j <= pole[0]; j++){
                bitset_setbit(pole,i*j,1);
            }
        }
    }
}