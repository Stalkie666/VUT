// prime.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <time.h>
#include "bitset.h"
#include "eratosthenes.h"

#define VELIKOST_POLE 230000000UL

int main(){
    clock_t start = clock();

    bitset_create(jmeno_pole,VELIKOST_POLE);

    Eratosthenes(jmeno_pole);

    //zde pridat logiku pro vytisknuti poslednich deseti cisel

    fprintf(stderr,"Time=%.3g\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}