#include <stdio.h>
#include <stdlib.h>
#include "proj2.h"

#define FILE_NAME proj2.out
#define ACTION_START 1

//pak ji nastavit na nulu
int * numberOfLine;

bool handle_arguments(int argc, char * argv[], arguments_t * arguments){
    if( argc != 6 ){
        fprintf(stderr,"Not expected number of argument.\n");
        return false;
    }

    //nezapomenout zkontrolovat spravnost vstupu
    arguments->numberOfCustomers = atoi(argv[1]);
    arguments->numberOfOfficials = atoi(argv[2]);
    arguments->maxTimeWaitCustomer = atoi(argv[3]);
    arguments->maxTimeOfficialsBreak = atoi(argv[4]);
    arguments->maxTimePostOfficeIsClosed = atoi(argv[5]);

}


int main(int argc, char * argv[]){
    setbuf(stdout, NULL);
    arguments_t arguments;
    if( !handle_arguments(argc,argv,&arguments) ){
        return EXIT_FAILURE;
    }


    printf("Test Proj2\n");
    return 0;
}