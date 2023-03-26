#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    returnStruct->lines = malloc(n*sizeof(singleLine_t));
    if(returnStruct->lines == NULL){
        free(returnStruct);
        return NULL;
    }
    returnStruct->size = n;
    returnStruct->readIndex = 0;
    returnStruct->writeIndex = 0;
    for(size_t i = 0; i < returnStruct->size; ++i){
        returnStruct->lines[i].line = NULL;
    }
    return returnStruct;
}

void cb_put(CB_Struct_t * cb, char * line){
    if(cb->lines[cb->writeIndex].line != NULL){
        free(cb->lines[cb->writeIndex].line);
    }
    cb->lines[cb->writeIndex++].line = line; 
}

char * cb_get(CB_Struct_t * cb){
    char * retVal = cb->lines[cb->readIndex++].line;
    cb->readIndex %= cb->size;
    return retVal;
}

void cb_free(CB_Struct_t * cb){
    //mozna jeste uvolnovat samotne line
    free(cb->lines);
    free(cb);
}

//read from stream
int readStream(FILE * stream, int allocSize){
    CB_Struct_t * cb = cb_create(allocSize);
    /*
        zde kod pro zpracovani vstupu
    */
    cb_free(cb);
    return 1;
}

//stdin mode
int readStdIn(int allocSize){
    return readStream(stdin,allocSize);
}
//read file mode
int readFromFile(int allocSize, char * fileName){
    FILE * stream = fopen(fileName,"r");
    if(!stream){
        fprintf(stderr,"Otevreni souboru selhalo.\n");
        return 0;
    }
    if( !readStream(stream,allocSize) ){
        fclose(stream);
        return 0;
    }
    else{
        fclose(stream);
    }
    return 1;
}



int main(int argc, char *argv[]){
    if(argc > 4)
        fprintf(stderr,"Prilis mnoho argumentu.\n");
    int CB_allocSize = 10;
    char * fileName = NULL;
    for(size_t i = 1; i < argc; ++i){
        if( strcmp(argv[i],"-n") && (i+1) < argc ){
            CB_allocSize = atoi(argv[i+1]);
            if( CB_allocSize <= 0 ){
                fprintf(stderr,"Zadana nespravna velikost CB, nebo uplny nesmysl.\n");
                return EXIT_FAILURE;
            }
        }
        else{
            fileName = argv[i];
        }
    }

    if( fileName == NULL ){
        //zde zavolat funkci pro cteni ze stdin
    }
    else{
        //zde funkce pro cteni ze souboru
    }

    return EXIT_SUCCESS;
}