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

bool init(){
    /* TO-DO */
    return true;
}



void process_customer(){
    // vypsat A: Z idZ: started

    //cekacka pomoci usleep random time mezi <0,TZ>

    // nyni rohodnuti
        // byl jsem linej zmrd a cekal moc dlouho a nasledkem toho musim domu
            // vypsat A: Z idZ: going home

        // nebo ne a ted budu srat urednika se svym pozadavkem
            // vypsat A: Z idZ: entering office for a service X

            // ceka az na nej zavola urednik

            // pote vypsat A: Z idZ: called by office worker

            // nasledne jsem liny hovado => usleep po random time <0,10>

            // Vypsat proces A: Z idZ: going home

            // a vypadnout z tohoto procesu kde v mainu na mne ceka smrt

}

void process_officials(){
    // na zacatku kazdej z tech zmrdu prijde do prace a vypise A: U idU: started

    // ted tu pry ten zmrd bude makat v cyklu dokud posta nezavre
        // pokud je nekdo v nejake fronte
            // jednu nahodne vybere a zni vytahne jednoho chudaka a zavola ho vypsanim A: U idU: serving a service of type X

            // pote je to liny zmrd a pomoci usleep ceka <0,10>

            // pote pokracuje v "praci"

        // pokud "nevidi" nikoho v zadne fronte, jde se ten zmetek ulit
            // vypise A: U idU: taking break

            // nasledne si ta lina svine pomuze funkci usleep a flaka se nahodne (tak urcite) v intervalu <0,TU>

            // potom laskave flkac vypise ze A: U idU: break finished

            // pote pokracuje v "praci"

    // posta zavrela a tak si ten zmrd rekl ze by mohl jit domu
        // pichne si hodiny a vypise A: U idU: going home

    // navraci se do main kde na toho zmrda po pravu ceka smrt
}

void process_postOffice(){
    // process posty nebo take Hlavni proces se spusti hned po inicializaci vseho potrebneho
    // odtud se budou forkovat vsechny child procesy

    // zacne tvorenim procesu pro zakazniky(cti pro ty zmrdy co furt otravujou)

    // pokracujeme vytvorenim tech zmrdu co se flakaji

    // nyni se bude flakat i budova samotna (ne neptejte se mne jak) po nahodnou dobu <F/2 , F>

    // potom si vzpomene ze by taky mohla skoncit a tak vypise A: closing 

    // potom ceka az uplne vsichni ti zbytecni lide ukonci svou existenci (asi pomoci semaforu) a ukonci vlastni existenci v main
}

void clean_up(){
    /* TO-DO */   
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