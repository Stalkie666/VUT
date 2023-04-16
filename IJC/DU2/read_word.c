#include <stdio.h>
#include <stdlib.h>
#include "read_word.h"

int readRestOfWord(FILE * stream){
    int retVal = 0;
    int c;
    while( (c=fgec(f)) != EOF ){
        if(isspace(c)){
            return i;
        }
        ++i;
    }
    return i;
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

    str[index] = '\0';

    if( (index==0) && (c == EOF) ){
        return EOF;
    }

    if( isspace(c)){
        return index;
    }

    return index + readRestOfWord(f);
}