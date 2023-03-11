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

int main(int argc, char *argv[]){
   if(argc != 2) error_exit("Chymbny pocet argumentu.\n");

   struct ppm * img = 
}