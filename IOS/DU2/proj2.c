#include <stdio.h>
#include <stdlib.h>
#include "proj2.h"

#define FILE_NAME "proj2.out"
#define ACTION_START 1

bool handle_arguments(int argc, char * argv[], arguments_t * arguments){
    if( argc != 6 ){
        fprintf(stderr,"Not expected number of arguments.\n");
        return false;
    }

    bool argFlag = true;

    //nezapomenout zkontrolovat spravnost vstupu
    arguments->numberOfCustomers = atoi(argv[1]);
    if( arguments->numberOfCustomers < 0 ) argFlag = false;
    arguments->numberOfOfficials = atoi(argv[2]);
    if(arguments->numberOfOfficials <= 0) argFlag = false;
    arguments->maxTimeWaitCustomer = atoi(argv[3]);
    if( arguments->maxTimeWaitCustomer < 0 || arguments->maxTimeWaitCustomer > 10000 ) argFlag = false;
    arguments->maxTimeOfficialsBreak = atoi(argv[4]);
    if( arguments->maxTimeOfficialsBreak < 0 || arguments->maxTimeOfficialsBreak > 100 ) argFlag = false;
    arguments->maxTimePostOfficeIsClosed = atoi(argv[5]);
    if( arguments->maxTimePostOfficeIsClosed < 0 || arguments->maxTimePostOfficeIsClosed >= 10000 ) argFlag = false;

    if(!argFlag)
        fprintf(stderr,"One or more arguments were wrongly entered.\n");

    return argFlag;
}


int main(int argc, char * argv[]){
    setbuf(stdout, NULL);
    arguments_t arguments;
    if( !handle_arguments(argc,argv,&arguments) ) exit(1);
    if(!init()){
        fprintf(stderr,"Initilazation failed.\n");
        exit(1);
    }


    printf("Test Proj2\n");
    exit(0);
}