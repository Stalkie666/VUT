// bitset.h
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <assert.h>

typedef unsigned long * bitset_t;
typedef unsigned long bitset_index_t;

//size of unsigned long
#define VELIKOST_LONG (sizeof(unsigned long)*CHAR_BIT)


//create and init static array
#define bitset_create(jmeno_pole,velikost)  static_assert(velikost > 0, "Zadana velikost je neplatna.\n");\
                                            unsigned long jmeno_pole[ (velikost/VELIKOST_LONG) + 1 +1] = {velikost,};

//create and init dynamic array
#define bitset_alloc(jmeno_pole,velikost)   int allocSize = (velikost / VELIKOST_LONG) + 1 + 1;\
                                            bitset_t jmeno_pole = (bitset_t)malloc(allocSize * sizeof(bitset_index_t));
//free dynamic array
#define bitset_free(jmeno_pole) free(jmeno_pole)


#ifndef USE_INLINE

    //get size of array
    #define bitset_size(jmeno_pole) (jmeno_pole[0])

    //help makro for check range of array
    #define check_range(jmeno_pole,index) (index < bitset_size(jmeno_pole) && index >= 0)

    //set bit
    #define bitset_setbit(jmeno_pole,index,vyraz) ( (check_range(jmeno_pole, index)) ? \
                                                    (   (vyraz) ? \
                                                        ( jmeno_pole[ (index / VELIKOST_LONG) + 1 ] |= vyraz << (index % VELIKOST_LONG) ) : \
                                                        ( jmeno_pole[ (index / VELIKOST_LONG) + 1 ] &= ~( vyraz <<  (index % VELIKOST_LONG) ) ) ) :\
                                                    (  error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index,(unsigned long)bitset_size(jmeno_pole)-1) ) )
    //get bit
    #define bitset_getbit(jmeno_pole,index) (   (check_range(jmeno_pole, index)) ? \
                                                ( (jmeno_pole[(index / VELIKOST_LONG)+1] >> (index % VELIKOST_LONG)) & 1 ) : \
                                                (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index,(unsigned long)bitset_size(jmeno_pole)-1) ) )

#endif



#ifdef USE_INLINE

inline bitset_index_t bitset_size(bitset_t jmeno_pole){
    return jmeno_pole[0];
}

inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, size_t vyraz){
    if( index >= bitset_size(jmeno_pole) ){
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index,(unsigned long)bitset_size(jmeno_pole)-1);
    }
    if(vyraz){
        jmeno_pole[ (index / VELIKOST_LONG) + 1 ] |= vyraz << (index % VELIKOST_LONG);
    }
    else{
        jmeno_pole[ (index / VELIKOST_LONG) + 1 ] &= ~( vyraz <<  (index % VELIKOST_LONG) );
    }
}

inline bitset_index_t bitset_getbit(bitset_t jmeno_pole, bitset_index_t index){
    if( index >= bitset_size(jmeno_pole) ){
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index,(unsigned long)bitset_size(jmeno_pole)-1);
    }
    return ( name[ (index / VELIKOST_LONG) + 1 ] >> (index % VELIKOST_LONG)) & 1 ;
}

#endif


//anonymn9 struktura - dodano helperem - asi nepouziju
//struct {int i;}nazev_promene = {.i = 42}; 


