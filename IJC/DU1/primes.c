// prime.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bitset.h"
#include "eratosthenes.h"

#define ARRAY_SIZE 200UL
#define NUMBERS_TO_PRINT 10

/**
 * print last N prime numbers
*/
void printLastPrimeNumbers(bitset_t array, bitset_index_t indexOfLastNumberOfArray, bitset_index_t numbersToPrint){
    //alloc of tmp data types
    bitset_t tmpArr = calloc(numbersToPrint,sizeof(bitset_index_t));
    if( tmpArr == NULL ){
        fprintf(stderr,"Memory allocation failure!\n");
        return;
    }
    bitset_index_t tmpNumToPrint = numbersToPrint;
    //search of latest prime numbers and store them in array
    while(tmpNumToPrint > 0 && indexOfLastNumberOfArray > 0){
        if( bitset_getbit(array,indexOfLastNumberOfArray) == 0 ){
            tmpArr[tmpNumToPrint-1] = indexOfLastNumberOfArray;
            --tmpNumToPrint;
        }
        --indexOfLastNumberOfArray;
    }
    //print latest prime numbers
    for(bitset_index_t i = 0; i < numbersToPrint; ++i ){
        printf("%lu\n", tmpArr[i]);
    }
    free(tmpArr);
}

int main(){
    //start counting
    clock_t start = clock();
    //define and init array
    bitset_create(arr,ARRAY_SIZE);
    //getting prime numbers with eratoshenes sieve
    Eratosthenes(arr);
    //print last N prime numbers
    printLastPrimeNumbers(arr, ARRAY_SIZE - 1, NUMBERS_TO_PRINT);
    //print how much time it takes
    fprintf(stderr,"Time=%.3g\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}