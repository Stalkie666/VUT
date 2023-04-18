// io.c
   // Řešení IJC-DU1, příklad b), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "io.h"

int readRestOfWord(FILE * stream){
    int retVal = 0;
    int tmp;
    while( (tmp=fgetc(stream)) != EOF ){
        if(isspace(tmp)){
            return retVal;
        }
        ++retVal;
    }
    return retVal;
}
/**
 * @return length of loaded word
*/
int read_word(char *s, int max, FILE *f){
    int index = 0;
    int tmp;
    // loading word char by char
    while( (tmp = fgetc(f)) != EOF ){
        // check if char is whitespace
        if( isspace(tmp) ){
            if( index != 0){
                break;
            }
        }
        // check if max length of word is reached
        else if(index == max-1){
            break;
        }
        // add char on the end of word
        else{
            s[index++] = tmp;
        }

    }
    //add terminating zero
    s[index] = '\0';
    // check EOF
    if( (tmp == EOF) && (index==0) ){
        return EOF;
    }
    // check if word has has correct length
    if( isspace(tmp)){
        return index;
    }
    // if word is longer, load rest of word and return full length of that word
    return index + readRestOfWord(f);
}