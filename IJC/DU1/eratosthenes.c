// erathosthenes.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <math.h>

#include "bitset.h"
#include "eratosthenes.h"

void Eratosthenes( bitset_t pole ){
    // cislo 0 a 1 nastavit na bit 1
    bitset_setbit(pole,0,1);
    bitset_setbit(pole,1,1);
    bitset_index_t maxNum = sqrt(pole[0]);
    bitset_index_t arraySize = bitset_size(pole);
    for(bitset_index_t i = 2; i < maxNum; ++i){
        //overeni validnosti cisla - bitu
        if(( bitset_getbit(pole,i)) == 0){
            //nastaveni vsech nasobku cisla
            for(bitset_index_t j = i*i; j < arraySize; j = j + i){
                (bitset_setbit(pole,j,1));
            }
        }
    }
}