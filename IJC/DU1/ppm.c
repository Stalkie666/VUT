// ppm.c
   // Řešení IJC-DU1, příklad b), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ppm.h"
#include "error.h"


bool isWhiteSpace(char c){
   return (  (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r') );
}

struct ppm * ppm_read(const char * filename){
   //open file and read it binary
   FILE * stream = fopen(filename,"rb");
   if( !stream ){
      warning("Otevření souboru: %s selhalo\n",filename);
      return NULL;
   }

   unsigned xsize;
   unsigned ysize;
   char arr[3];
   unsigned maxVal;
   //load data about size of picture and 
   if(   fscanf(stream, "%2s %u %u %u", arr, &xsize, &ysize, &maxVal) != 4 ||
         maxVal != 255 ||
         !isWhiteSpace( fgetc(stream) ) ||
         strcmp(arr,"P6")  ){
            warning("Soubor neodpovida formatu P6\n");
            fclose(stream);
            return NULL;
         }
   //allocation of structure and data array      
   size_t allocSize = 3*xsize*ysize;
   struct ppm * retVal = malloc(sizeof(struct ppm) );
   if( !retVal ){
      fclose(stream);
      return NULL;
   }
   retVal->xsize = xsize;
   retVal->ysize = ysize;
   retVal->data = malloc(allocSize*sizeof(char));
   if( !(retVal->data) ){
      fclose(stream);
      free(retVal);
      return NULL;
   }
   //copy data from stream to structure
   size_t realSize = fread( retVal->data, sizeof(char), allocSize, stream );
   //check if success
   if( (fgetc(stream) != EOF) || realSize < allocSize ){
      fclose(stream);
      warning("Nespravna velikost souboru\n");
      free(retVal->data);
      free(retVal);
      return NULL;
   }
   fclose(stream);
   return retVal;
}


void ppm_free(struct ppm *p){
   free(p->data);
   free(p);
}
