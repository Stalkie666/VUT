// error.c
   // Řešení IJC-DU1, příklad b), 20.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void warning(const char *fmt, ...){
   va_list args;
   va_start(args, fmt);
   fprintf(stderr, "Warning: ");
   vfprintf(stderr, fmt, args);
   va_end(args);
}

void error_exit(const char *fmt, ...){
   va_list args;
   va_start(args, fmt);
   fprintf(stderr, "Error: ");
   vfprintf(stderr, fmt, args);
   va_end(args);
   exit(1);
}