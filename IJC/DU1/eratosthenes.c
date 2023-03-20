// erathosthenes.c
   // Řešení IJC-DU1, příklad a), 20.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <math.h>
#include "eratosthenes.h"

void Eratosthenes( bitset_t pole ){
    // number 0 and 1 set bit to 1
    bitset_setbit(pole,0,1);
    bitset_setbit(pole,1,1);
    bitset_index_t maxNum = sqrt(pole[0]);
    bitset_index_t arraySize = bitset_size(pole);
    for(bitset_index_t i = 2; i < maxNum; ++i){
        //check if number (bit) is valid (0)
        if(( bitset_getbit(pole,i)) == 0){
            //set all multiplies of number to 1 (not prime)
            for(bitset_index_t j = 2; j*i < arraySize; j++){
                (bitset_setbit(pole,(j*i),1));
            }
        }
    }
}