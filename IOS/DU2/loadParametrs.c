#include <string.h>
#include <stdlib.h>
#include "loadParametrs.h"

bool loadParameters(parameters_t * parametrs, int nParametrs,const char * arguments[]){
    if( nParametrs != 6 )
        return false;

    parametrs->pocetZakazniku = atoi( arguments[1] );
    if( parametrs->pocetZakazniku < 0 ) 
        return false;

    parametrs->pocetUredniku = atoi( arguments[2] );
    if( parametrs->pocetUredniku < 0 ) 
        return false;

    parametrs->maxCasCekaniZakaznika = atoi( arguments[3] );
    if( parametrs->maxCasCekaniZakaznika < 0 || parametrs->maxCasCekaniZakaznika > 10000 )
        return false;

    parametrs->maxDelkaPrestavkyUrednika = atoi( arguments[4] );
    if( parametrs->maxDelkaPrestavkyUrednika < 0 || parametrs->maxDelkaPrestavkyUrednika > 100 )
        return false;

    parametrs->maxDobaUzavreniPosty = atoi( arguments[5] );
    if(parametrs->maxDobaUzavreniPosty < 0 || parametrs->maxDobaUzavreniPosty > 10000)
        return false;

    return true;
}