// bitset.c
   // Řešení IJC-DU1, příklad a), 20.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include "bitset.h"

#ifdef USE_INLINE
    extern bitset_index_t bitset_size(bitset_t jmeno_pole);
    extern void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, size_t vyraz);
    extern bitset_index_t bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);
#endif