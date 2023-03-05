// ppm.c
   // Řešení IJC-DU1, příklad b), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
#include "error.h"

struct ppm * ppm_read(const char * filename){
   //otevre soubor pro cteni binarne
   FILE * stream = fopen(filename,"rb");
   if( !stream ){
      warning("Otevření souboru: %s selhalo\n",filename);
      return NULL;
   }

      // TO-DO
      //dodelat

}


void ppm_free(struct ppm *p){
   free(p);
}
