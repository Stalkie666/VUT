// bitset.h
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

typedef unsigned long bitset_index_t;
typedef struct bitset
{
    bitset_index_t * array;
    bitset_index_t realSize;
    bitset_index_t allocSize;
    bitset_index_t bit_velikost
}bitset_t;

//sem nejak pridat logiku pro zjisteni jestli se jedna o 64bit nebo 32bit
    //a podle toho prispusobit #define
    //mozna dokonce duplikovat prikazi nebo alespon casti kvuli tomuto, 
    //spise ty casti pres else if nebo tak nejak

#define bitset_create(jmeno_pole,velikost) bitset_t jmeno_pole

#define bitset_alloc(jmeno_pole,velikost)

#define bitset_free(jmeno_pole) free(jmeno_pole.array)

#define bitset_size(jmeno_pole) jmeno_pole.bit_velikost

#define bitset_setbit(jmeno_pole,index,vyraz)

#define bitset_getbit(jmeno_pole,index)
