// bitset.h
   // Řešení IJC-DU1, příklad a), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

typedef unsigned long * bitset_t;
typedef unsigned long bitset_index_t;

//dodano helperem
#define VELIKOST sizeof(unsigned long)


//sem nejak pridat logiku pro zjisteni jestli se jedna o 64bit nebo 32bit
    //a podle toho prispusobit #define
    //mozna dokonce duplikovat prikazi nebo alespon casti kvuli tomuto, 
    //spise ty casti pres else if nebo tak nejak

//na z8sobn9ku
#define bitset_create(jmeno_pole,velikost)  unsigned long staticArr[velikost/sizeof(unsigned long) + 1 +1];\
                                            bitset_t jmeno_pole = staticArr;
//na halde
#define bitset_alloc(jmeno_pole,velikost)   int allocSize = velikost / sizeof(unsigned long) + 1 + 1;\
                                            bitset_t jmeno_pole = (unsigned long *)malloc(allocSize * sizeof(unsigned long));

#define bitset_free(jmeno_pole) free(jmeno_pole)

#define bitset_size(jmeno_pole) jmeno_pole[0]

#define bitset_setbit(jmeno_pole,index,vyraz)

#define bitset_getbit(jmeno_pole,index)


//anonymn9 struktura - dodano helperem
struct {int i;}nazev_promene = {.i = 42}; 
