// tail.c
   // Řešení IJC-DU1, příklad a), 17.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 11.3.0
   
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LIMIT 4096 // 4095 printable chars + '\0'
#define BASE_ROW_LIMIT 10

typedef struct singleLine{
    char * line;
}singleLine_t;

typedef struct CB_Struct{
    size_t size;            //size of array
    size_t readIndex;       //index on next line for read
    size_t writeIndex;      //index on next line for write
    singleLine_t * lines;   //array of lines
}CB_Struct_t;

/**
 * @return pointer for structure if success, NULL if not
*/
CB_Struct_t * cb_create(int n){
    // alloc structure and check if succesfully
    CB_Struct_t * returnStruct = malloc(sizeof(CB_Struct_t));
    if( returnStruct == NULL ) return NULL;
    // alloc array of lines and check if succesfully
    returnStruct->lines = malloc(n*sizeof(singleLine_t));
    if(returnStruct->lines == NULL){
        free(returnStruct);
        return NULL;
    }
    //init variables in structure and return
    returnStruct->size = n;
    returnStruct->readIndex = 0;
    returnStruct->writeIndex = 0;
    for(size_t i = 0; i < returnStruct->size; ++i){
        returnStruct->lines[i].line = NULL;
    }
    return returnStruct;
}

/**
 * add new line into array
*/
void cb_put(CB_Struct_t * cb, char * line){
    // move readIndex if nessessary
    if( (cb->writeIndex == cb->readIndex) ){
        cb->readIndex = (cb->readIndex + 1) % cb->size;
    }
    // if writeIndex is occupied, remove line correctly
    if(cb->lines[cb->writeIndex].line != NULL){
        free(cb->lines[cb->writeIndex].line);
    }
    // set new line
    cb->lines[cb->writeIndex++].line = line;
    // correct write index if nessessary
    cb->writeIndex %= cb->size;
}

/**
 * @return line of array on which readIndex is pointing 
*/
char * cb_get(CB_Struct_t * cb){
    char * retVal = cb->lines[cb->readIndex++].line;
    cb->readIndex %= cb->size;
    return retVal;
}

/**
 * free structure
*/
void cb_free(CB_Struct_t * cb){
    // free array of lines if it is needed
    for(size_t i = 0; i < cb->size; ++i){
        if( cb->lines[i].line != NULL ){
            free(cb->lines[i].line);
        }
    }
    // free array and structure
    free(cb->lines);
    free(cb);
}

//read from stream
int readStream(FILE * stream, int allocSize){
    // alloc structure
    CB_Struct_t * cb = cb_create(allocSize);
    //check first line over size
    int overSizeLine = 0;

    char * line = NULL;
    size_t len = 0;
    // read stream until EOF
    while( !feof(stream) ){
        line = NULL;
        len = 0;
        int realLineSize = getline(&line,&len,stream);
        // check if line is loaded and if length is under limit
        if( realLineSize != -1  && realLineSize < LINE_LIMIT){
            if(line[realLineSize-1] == '\n') 
                line[realLineSize-1] = '\0';
            // put line into array
            cb_put(cb,line);
        }
        // check if line is over limit
        else if( realLineSize >= LINE_LIMIT ){
            //shorter line
            line[LINE_LIMIT-1] = '\0';
            // print error message
            if(overSizeLine == 0){
                overSizeLine = 1;
                fprintf(stderr,"Prilis dlouhy radek (vice nez %d znaku), radek zkracen.\n",LINE_LIMIT);
            }
            // put line into array
            cb_put(cb,line);
        }
        // if line is empty
        else{
            // if nessesary free line
            if(len > 0) free(line);
        }
    }
    // print last n lines given by arguments
    for(size_t i = 0; i < cb->size; ++i){
        char * testLine = cb_get(cb);
        // check if line is not empty
        if( testLine != NULL )
            printf("%s\n", testLine);
    }
    // free structure
    cb_free(cb);
    return 1;
}

//stdin mode
int readStdIn(int allocSize){
    return readStream(stdin,allocSize);
}
//read file mode
int readFromFile(int allocSize, char * fileName){
    //try to open file and check
    FILE * stream = fopen(fileName,"r");
    if(!stream){
        fprintf(stderr,"Otevreni souboru selhalo.\n");
        return 0;
    }
    // read stream and return if successfuly or not
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
    // check number of arguments
    if(argc > 4){
        // print error message
        fprintf(stderr,"Prilis mnoho argumentu.\n");
        return EXIT_FAILURE;
    }
    // set variables
    int CB_allocSize = BASE_ROW_LIMIT;
    char * fileName = NULL;
    // processing of given arguments
    for(int i = 1; i < argc; ++i){
        // for number of rows to print
        if( !strcmp(argv[i],"-n") ){
            // check if after '-n' is argument
            if( i+1 >= argc ){
                fprintf(stderr,"Chyby cislo pro argument -n\n");
                return EXIT_FAILURE;
            }
            // convers argument into number
            CB_allocSize = atoi(argv[i+1]);
            if( CB_allocSize <= 0 ){
                fprintf(stderr,"Zadana nespravna velikost CB, nebo uplny nesmysl.\n");
                return EXIT_FAILURE;
            }
            ++i;
        }
        // for fileName
        else{
            fileName = argv[i];
        }
    }

    // check if fileName was given
    if( fileName == NULL ){
        // read input from stdin
        if( readStdIn(CB_allocSize) == 0 )
            return EXIT_FAILURE;
    }
    else{
        // read input from file
        if( readFromFile(CB_allocSize,fileName) == 0 )
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}