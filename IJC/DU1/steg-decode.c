// steg-decode.c
   // Řešení IJC-DU1, příklad b), 20.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "eratosthenes.h"
#include "bitset.h"
#include "error.h"
#include "ppm.h"

#define START_PRIME 101
#define MAX_MESSAGE_LENGTH 10000

//disclaimer: vzato podle pokynů v zadání, kde je napsáno: (použijte např.: https://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c)
/*
 * The utf8_check() function scans the '\0'-terminated string starting
 * at s. It returns a pointer to the first byte of the first malformed
 * or overlong UTF-8 sequence found, or NULL if the string contains
 * only correct UTF-8. It also spots UTF-8 sequences that could cause
 * trouble if converted to UTF-16, namely surrogate characters
 * (U+D800..U+DFFF) and non-Unicode positions (U+FFFE..U+FFFF). This
 * routine is very likely to find a malformed sequence if the input
 * uses any other encoding than UTF-8. It therefore can be used as a
 * very effective heuristic for distinguishing between UTF-8 and other
 * encodings.
 *
 * I wrote this code mainly as a specification of functionality; there
 * are no doubt performance optimizations possible for certain CPUs.
 *
 * Markus Kuhn <http://www.cl.cam.ac.uk/~mgk25/> -- 2005-03-30
 * License: http://www.cl.cam.ac.uk/~mgk25/short-license.html
 */
unsigned char *utf8_check(unsigned char *s)
{
  while (*s) {
    if (*s < 0x80)
      /* 0xxxxxxx */
      s++;
    else if ((s[0] & 0xe0) == 0xc0) {
      /* 110XXXXx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[0] & 0xfe) == 0xc0)                        /* overlong? */
	return s;
      else
	s += 2;
    } else if ((s[0] & 0xf0) == 0xe0) {
      /* 1110XXXX 10Xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||    /* surrogate? */
	  (s[0] == 0xef && s[1] == 0xbf &&
	   (s[2] & 0xfe) == 0xbe))                      /* U+FFFE or U+FFFF? */
	return s;
      else
	s += 3;
    } else if ((s[0] & 0xf8) == 0xf0) {
      /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[3] & 0xc0) != 0x80 ||
	  (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4) /* > U+10FFFF? */
	return s;
      else
	s += 4;
    } else
      return s;
  }

  return NULL;
}



int main(int argc, char *argv[]){
   //check number of arguments
   if(argc != 2) error_exit("Chybny pocet argumentu.\n");

   //prepare message and check for end bit
   unsigned char message[MAX_MESSAGE_LENGTH] = {0};
   //for counting bits
   size_t messageIndex = 0;
   //for check if message is ended
   int isMsgEnded = 0;

   //load data from picture
   struct ppm * img = ppm_read(argv[1]);
   if( !img ) error_exit("Nacteni obrazku selhalo.\n");

   //load mask
   size_t maskSize = 3*img->xsize*img->ysize;
   bitset_alloc(maskArray,maskSize);
   Eratosthenes(maskArray);



   for(size_t i = START_PRIME; i < maskSize; ++i ){
      //check if is a primebit
      if( !bitset_getbit(maskArray,i) ){
         //read last bit
         int tmpBit = img->data[i] & 1;
         message[messageIndex / CHAR_BIT] |= (char)tmpBit << (messageIndex % CHAR_BIT);
         ++messageIndex;

         //check if is whole char
         if( messageIndex % CHAR_BIT == 0 ){
            //check for '\0'
            if( message[ (messageIndex / CHAR_BIT) - 1 ] == '\0' ){
               isMsgEnded = 1;
               break;
            }
            //check for overflow array
            if( messageIndex / CHAR_BIT >= MAX_MESSAGE_LENGTH ){
               break;
            }
         }
      }

   }

   bitset_free(maskArray);
   ppm_free(img);
   //check if super-duper secret message was ended by '\0'
   if( !isMsgEnded ){
      error_exit("Skryta zprava je prilis dlouha nebo je neukoncena terminacni nulou.\n");
   }
   //check if message is in UTF8 standard - copied from: https://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c
   if(utf8_check(message) != NULL){
      error_exit("Skryta zprava neni pravdepodobne v UTF8 formatu.\n");
   }

   printf("%s\n",message);
   return 0;
}