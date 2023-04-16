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

int read_word(char *s, int max, FILE *f){
    int index = 0;
    int c;
    while( (c = fgetc(f)) != EOF ){
        if( isspace(c) ){
            if( index != 0){
                break;
            }
        }
        else if(index == max-1){
            break;
        }
        else{
            s[index++] = c;
        }

    }

    s[index] = '\0';

    if( (index==0) && (c == EOF) ){
        return EOF;
    }

    if( isspace(c)){
        return index;
    }

    return index + readRestOfWord(f);
}