#include <stdbool.h>

typedef struct parametrs
{
    int pocetZakazniku;
    int pocetUredniku;
    int maxCasCekaniZakaznika;
    int maxDelkaPrestavkyUrednika;
    int maxDobaUzavreniPosty;
}parameters_t;

bool loadParameters(parameters_t * parametrs, int nParametrs,const char * arguments[]);