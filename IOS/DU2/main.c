#include <stdio.h>
#include <stdlib.h>
#include "loadParametrs.h"

int main(int argc,const char * argv[]){
    parameters_t parametry;
    if( !loadParameters(&parametry,argc,argv) ){
        fprintf(stderr,"Chybne zadane argumenty.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}