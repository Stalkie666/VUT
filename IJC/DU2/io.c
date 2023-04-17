#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "io.h"

int readRestOfWord(FILE * stream){
    int retVal = 0;
    int c;
    while( (c=fgetc(stream)) != EOF ){
        if(isspace(c)){
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
    int c;
    // loading word char by char
    while( (c = fgetc(f)) != EOF ){
        // check if char is whitespace
        if( isspace(c) ){
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
            s[index++] = c;
        }

    }
    //add terminating zero
    s[index] = '\0';
    // check EOF
    if( (index==0) && (c == EOF) ){
        return EOF;
    }
    // check if word has has correct length
    if( isspace(c)){
        return index;
    }
    // if word is longer, load rest of word and return full length of that word
    return index + readRestOfWord(f);
}