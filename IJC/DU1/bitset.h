// bitset.h
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <assert.h>

typedef unsigned long * bitset_t;
typedef unsigned long bitset_index_t;

//dodano helperem
#define VELIKOST_LONG (sizeof(unsigned long)*CHAR_BIT)


//sem nejak pridat logiku pro zjisteni jestli se jedna o 64bit nebo 32bit
    //a podle toho prispusobit #define
    //mozna dokonce duplikovat prikazi nebo alespon casti kvuli tomuto, 
    //spise ty casti pres else if nebo tak nejak

//na z8sobn9ku
#define bitset_create(jmeno_pole,velikost)  static_assert(velikost > 0, "Zadana velikost je neplatna.\n");\
                                            unsigned long jmeno_pole[ (velikost/VELIKOST_LONG) + 1 +1] = {velikost,};

//na halde
#define bitset_alloc(jmeno_pole,velikost)   int allocSize = (velikost / VELIKOST_LONG) + 1 + 1;\
                                            bitset_t jmeno_pole = (bitset_t)malloc(allocSize * sizeof(bitset_index_t));

#define bitset_free(jmeno_pole) free(jmeno_pole)

#define bitset_size(jmeno_pole) (jmeno_pole[0])

//pomocna na zjisteni, jestli je cislo v poli
#define check_range(jmeno_pole,index) (index < bitset_size(jmeno_pole) && index >= 0)

#define bitset_setbit(jmeno_pole,index,vyraz) ( check_range(jmeno_pole, index) ? /*nastavit bit*/ : /*zavolat error*/ )

#define bitset_getbit(jmeno_pole,index) ( check_range(jmeno_pole, index) ? /*ziskat bit*/ : /*zavolat error*/ )


//anonymn9 struktura - dodano helperem - asi nepouziju
struct {int i;}nazev_promene = {.i = 42}; 


