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
