// steg-decode.c
   // Řešení IJC-DU1, příklad b), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <limits.h>

#include "eratosthenes.h"
#include "bitset.h"
#include "error.h"
#include "ppm.h"

#define START_PRIME 101
#define MAX_MESSAGE_LENGTH 10000

int main(int argc, char *argv[]){
   //prepare message and check for end bit
   char message[MAX_MESSAGE_LENGTH] = {0};
   size_t messageIndex = 0;
   int isMsgEnded = 0;

   //check number of arguments
   if(argc != 2) error_exit("Chybny pocet argumentu.\n");

   //load data from picture
   struct ppm * img = ppm_read(argv[1]);
   if( !img ) error_exit("Nacteni obrazku selhalo.\n");

   //load mask
   size_t maskSize = 3*img->xsize*img->ysize;
   bitset_alloc(maskArray,maskSize);
   Eratosthenes(maskArray);



   for(size_t i = START_PRIME; i < maskSize; ++i ){
      if( !bitset_getbit(maskArray,i) ){
         int tmpBit = img->data[i] & 1;
         message[messageIndex / CHAR_BIT] |= (char)tmpBit << (messageIndex % CHAR_BIT);
         ++messageIndex;

         if( messageIndex % CHAR_BIT == 0 ){
            if( message[ (messageIndex / CHAR_BIT) - 1 ] == '\0' ){
               isMsgEnded = 1;
               break;
            }
            if( messageIndex / CHAR_BIT >= MAX_MESSAGE_LENGTH ){
               break;
            }
         }
      }

   }

   bitset_free(maskArray);
   ppm_free(img);

   if( !isMsgEnded ){
      error_exit("Skryta zprava je prilis dlouha nebo je neukoncena terminacni nulou.\n");
   }
   printf("%s\n",message);
   return 0;
}