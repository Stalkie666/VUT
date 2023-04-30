#include "proj2.h"

#define FILE_NAME "proj2.out"
#define ACTION_START 1

void print_message(char * format, ...){
    sem_wait(s_write);
    va_list args;
    va_start(args, format);
    fprintf(streamFile,"%d: ",*lineCounter);
    *lineCounter += 1;
    vfprintf(streamFile,format,args);
    fflush(streamFile);
    va_end(args);
    sem_post(s_write);
}

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
    if( arguments->maxTimePostOfficeIsClosed < 0 || arguments->maxTimePostOfficeIsClosed > 10000 ) argFlag = false;

    if(!argFlag)
        fprintf(stderr,"One or more arguments were wrongly entered.\n");

    return argFlag;
}

bool init(){
    streamFile = fopen(FILE_NAME,"w");
    if(streamFile == NULL){
        fprintf(stderr, "Opening file failed. Program will be terminated.\n");
        exit(1);
    }

    //init semafory
    s_write = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_task1 = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_task2 = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_task3 = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_taskContinue = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_officialChoosing = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    s_allowOfficiesGoHome = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0,0);
    sem_init(s_write,1,1);
    sem_init(s_task1,1,0);
    sem_init(s_task2,1,0);
    sem_init(s_task3,1,0);
    sem_init(s_taskContinue,1,0);
    sem_init(s_officialChoosing,1,1);
    sem_init(s_allowOfficiesGoHome,1,0);
    //nastaveni sdilene pameti
    lineCounter= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    *lineCounter = 1;
    customersWaiting= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    *customersWaiting = 0;
    isPostOfficeOpen= mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    *isPostOfficeOpen = true;
    frontCounter= mmap(NULL, sizeof(frontCounter_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    frontCounter->front1 = 0;
    frontCounter->front2 = 0;
    frontCounter->front3 = 0;
    /* TO-DO */
    return true;
}

void clean_up(){
    fclose(streamFile);
    /* TO-DO */   
    sem_destroy(s_write);
    sem_destroy(s_task1);
    sem_destroy(s_task2);
    sem_destroy(s_task3);
    sem_destroy(s_taskContinue);
    sem_destroy(s_officialChoosing);
    sem_destroy(s_allowOfficiesGoHome);
    // semafory
    munmap(s_write,sizeof(sem_t));
    munmap(s_task1,sizeof(sem_t));
    munmap(s_task2,sizeof(sem_t));
    munmap(s_task3,sizeof(sem_t));
    munmap(s_taskContinue,sizeof(sem_t));
    munmap(s_officialChoosing,sizeof(sem_t));
    munmap(s_allowOfficiesGoHome,sizeof(sem_t));


    //sdilenu pamet
    munmap(lineCounter,sizeof(int));
    munmap(customersWaiting,sizeof(int));
    munmap(isPostOfficeOpen,sizeof(bool));
    munmap(frontCounter,sizeof(frontCounter_t));
}



void process_customer(int CustomerId, int maxTimeToWait){
    srand(getpid());
    // vypsat A: Z idZ: started
    print_message("Z %d: started\n",CustomerId);
    //cekacka pomoci usleep random time mezi <0,TZ>
    usleep( rand() % (maxTimeToWait+1) );
    // nyni rohodnuti jestli mam peska nebo budu srat urednika se svym pozadavkem
    if( *isPostOfficeOpen == true ){
        int choosenService = rand() % 3 + 1;
        *customersWaiting += 1;
        // vypsat A: Z idZ: entering office for a service X
        print_message("Z %d: entering office for a service %d\n",CustomerId,choosenService);
        // vybrat do ktere fronty se zaradim
        // ceka az na nej zavola urednik
        switch (choosenService)
        {
        case 1:
            frontCounter->front1 += 1;
            sem_wait(s_task1);
            frontCounter->front1 -= 1;
            break;
        case 2:
            frontCounter->front2 += 1;
            sem_wait(s_task2);
            frontCounter->front2 -= 1;
            break;
        case 3:
            frontCounter->front3 += 1;
            sem_wait(s_task3);
            frontCounter->front3 -= 1;
            break;
        default:
            break;
        }
        *customersWaiting -= 1;
        sem_post(s_taskContinue);

        // pote vypsat A: Z idZ: called by office worker
        print_message("Z %d: called by office worker\n",CustomerId);
        // nasledne jsem liny hovado => usleep po random time <0,10>
        usleep( rand() % 11 );
    }
    // Vypsat proces A: Z idZ: going home
    print_message("Z %d: going home\n",CustomerId);
    // a vypadnout z tohoto procesu kde v mainu na mne ceka smrt
    return;
}

void process_officials(int OfficialId, int maxTimeForBreak){
    srand(getpid());
    // na zacatku kazdej z tech zmrdu prijde do prace a vypise A: U idU: started
    print_message("U %d: started\n",OfficialId);
    // ted tu pry ten zmrd bude makat v cyklu dokud posta nezavre a neobslouzi vsechny zakazniky
    while( *isPostOfficeOpen == true || *customersWaiting > 0 ){

        // pokud je nekdo v nejake fronte
        sem_wait(s_officialChoosing);
        if( *customersWaiting > 0 ){
            // jednu nahodne vybere a zni vytahne jednoho chudaka a zavola ho vypsanim A: U idU: serving a service of type X
            int index = rand() % 3;
            bool isChosen = false;
            do
            {
                switch (index)
                {
                case 0:
                    if(frontCounter->front1 > 0){
                        sem_post(s_task1);
                        isChosen = true;
                    }
                    else 
                        index = (index + 1) % 3;
                    break;
                case 1:
                    if(frontCounter->front2 > 0){
                        sem_post(s_task2);
                        isChosen = true;
                    }
                    else 
                        index = (index + 1) % 3;
                    break;
                case 2:
                    if(frontCounter->front3 > 0){
                        sem_post(s_task3);
                        isChosen = true;
                    }
                    else 
                        index = (index + 1) % 3;
                    break;
                default:
                    break;
                }
            } while ( isChosen == false );

            print_message("U %d: serving a service of type %d\n",OfficialId,(index+1));
            sem_wait(s_taskContinue);
            sem_post(s_officialChoosing);
            // pote je to liny zmrd a pomoci usleep ceka <0,10>
            usleep( rand() % 11 );
            // pote oznami ze te teda laskave doobslouzil U idU: service finished
            print_message("U %d: service finished\n",OfficialId);
            // pote pokracuje v "praci"
        }
        // pokud "nevidi" nikoho v zadne fronte, jde se ten zmetek ulit
        else{
            sem_post(s_officialChoosing);
            // vypise A: U idU: taking break
            print_message("U %d: taking break\n",OfficialId);
            // nasledne si ta lina svine pomuze funkci usleep a flaka se nahodne (tak urcite) v intervalu <0,TU>
            usleep( rand() % (maxTimeForBreak+1) );
            // potom laskave flakac vypise ze A: U idU: break finished
            print_message("U %d: break finished\n", OfficialId);
            // pote pokracuje v "praci"
        }
    }
    // posta zavrela a uz "obslouzil" vsechny ctene zakazniky a tak si ten zmrd rekl ze by mohl jit domu
        // pichne si hodiny a vypise A: U idU: going home
    sem_wait(s_allowOfficiesGoHome);
    print_message("U %d: going home\n",OfficialId);
    sem_post(s_allowOfficiesGoHome);
    
    // navraci se do main kde na toho zmrda po pravu ceka smrt
    return;
}

void process_postOffice(arguments_t * arguments){
    *isPostOfficeOpen = true;
    // process posty nebo take Hlavni proces se spusti hned po inicializaci vseho potrebneho
    // odtud se budou forkovat vsechny child procesy
    // zacne tvorenim procesu pro zakazniky(cti pro ty zmrdy co furt otravujou)
    for( int i = 1; i <= arguments->numberOfCustomers; ++i  ){
        pid_t id = fork();
        if( id == 0 ){
            process_customer(i,arguments->maxTimeWaitCustomer);
            exit(0);
        }
    }
    // pokracujeme vytvorenim tech zmrdu co se flakaji - uredniku
    for( int i = 1; i <= arguments->numberOfOfficials; ++i ){
        pid_t id = fork();
        if( id == 0 ) {
            process_officials(i,arguments->maxTimeOfficialsBreak);
            exit(0);
        }
    }

    // nyni se bude flakat i budova samotna (ne neptejte se mne jak) po nahodnou dobu <F/2 , F>
    int halfWait =  arguments->maxTimePostOfficeIsClosed / 2;
    srand(getpid());
    usleep( ( rand() % halfWait) + halfWait );
    // potom si vzpomene ze by taky mohla skoncit a tak vypise A: closing 
    *isPostOfficeOpen = false;
    print_message("closing\n");
    sem_post(s_allowOfficiesGoHome);
    // potom ceka az uplne vsichni ti zbytecni lide ukonci svou existenci (asi pomoci semaforu) a ukonci vlastni existenci v main
    while(wait(NULL) > 0);
    return;
}



int main(int argc, char * argv[]){
    setbuf(stdout, NULL);
    arguments_t arguments;
    if( !handle_arguments(argc,argv,&arguments) ) exit(1);
    if(!init()){
        fprintf(stderr,"Initilazation failed.\n");
        exit(1);
    }

    process_postOffice(&arguments);

    clean_up();
    exit(0);
}