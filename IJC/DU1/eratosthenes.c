// erathosthenes.c
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include "bitset.h"

//zjijstit jestli se jedna o 32bit nebo o 64bit verzi

//projit jednotliva cisla v poli po bitech, pouzit bitovy posuv
    //filter bude unsigned long stejne jako pole.array[] 
    // asi nejak takhle: ,filter=1; porovnam; filter << 1 - bit posuv; a znova
    //jeste nekde ukladat se kterym cislem to porovnavam
    //asi proste budu odecitat, nejjedodussi

//po dokonceni nulovani cisel co nejsou prvocisla, 
    //vytisknout 10 poslednich(?pomoci rekurze asi,ale slo by to i jinak)


void Eratosthenes( bitset_t pole ){

}