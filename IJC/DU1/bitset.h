// bitset.h
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

typedef struct bitset
{
    unsigned long * array;
    unsigned long realSize;
    unsigned long allocSize;
}bitset_t;

typedef unsigned long bitset_index_t;

#define bitset_create(jmeno_pole,velikost)

#define bitset_alloc(jmeno_pole,velikost)

#define bitset_free(jmeno_pole)

#define bitset_size(jmeno_pole)

#define bitset_setbit(jmeno_pole,index,vyraz)

#define bitset_getbit(jmeno_pole,index)
