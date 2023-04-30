// proj2.h
   // Řešení IOS-DU2 30.4.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <semaphore.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//struktura pro zachyceni pocatecnich argumentu
typedef struct arguments{
    int numberOfCustomers;
    int numberOfOfficials;
    int maxTimeWaitCustomer;
    int maxTimeOfficialsBreak;
    int maxTimePostOfficeIsClosed;
}arguments_t;

//struktura pro pocitani lidi v jednotlivych frontach
typedef struct frontCounterStruct{
    int front1;
    int front2;
    int front3;
}frontCounter_t;

//semafory
sem_t * s_write;
sem_t * s_task1;        //cekani v ve fronte 1
sem_t * s_task2;        //cekani v ve fronte 2
sem_t * s_task3;        //cekani v ve fronte 3
sem_t * s_taskContinue; //dokonceni decrementace
sem_t * s_officialChoosing; //cekani az si uradnik vybere frontu nebo prestavku
sem_t * s_allowOfficiesGoHome;

FILE *streamFile = NULL;    // ukazatel na stream z otevreneho souboru
int *lineCounter = NULL;    // ve sdilene pameti, pamatuje si kolikaty radek se tiskne do souboru
frontCounter_t * frontCounter = NULL;   // zaznamenava kolik lidi je v jednotlivych radach
int *customersWaiting = NULL;   // zaznamenava kolik lidi je celkove v radach
bool *isPostOfficeOpen = NULL;  // flag jestli je este postOffice otevrena

// pomocna funkce na vytisteni zprav
void print_message(char * format, ...);
// pomocna funkce na zpracovani prichozich argumentu
bool handle_arguments(int argc, char * argv[], arguments_t * arguments);
// funkce na inicializaci promenych do sdilene pameti a inicializaci semaforu
bool init();
// funkce na uklid pred skoncenim programu, uvolneni alokovane pameti, zniceni semaforu...
void clean_up();
// funkce ktera zpracovava process zakaznika
void process_customer(int CustomerId, int maxTimeToWait);
// funkce ktera zpracovava process urednika
void process_officials(int OfficialId, int maxTimeForBreak);
// funkce ktera vytvari jednotlive procesy zakazniku a uredniku a ovlada funkce posty
void process_postOffice(arguments_t * arguments);
