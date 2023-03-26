#include <stdio.h>
#include <stdlib.h>

#define LINE_LIMIT 4096 // 4095 printable chars + '\0'

typedef struct singleLine{
    char * line;
}singleLine_t;

typedef struct CB_Struct{
    size_t size;
    size_t readIndex;
    size_t writeIndex;
    singleLine_t * lines;
}CB_Struct_t;

CB_Struct_t * cb_create(int n){
    CB_Struct_t * returnStruct = malloc(sizeof(CB_Struct_t));
    if( returnStruct == NULL ) return NULL;
    returnStruct->lines = malloc(n*sizeof(singleLine));
    if(returnStruct->lines == NULL){
        free(returnStruct);
        return NULL
    }
    returnStruct->size = n;
    returnStruct->readIndex = 0;
    returnStruct->writeIndex = 0;
    return returnStruct;
}

int main(){
    return 0;
}