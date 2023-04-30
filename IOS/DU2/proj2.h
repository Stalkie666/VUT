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

typedef struct arguments{
    int numberOfCustomers;
    int numberOfOfficials;
    int maxTimeWaitCustomer;
    int maxTimeOfficialsBreak;
    int maxTimePostOfficeIsClosed;
}arguments_t;

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

FILE *streamFile = NULL;
int *lineCounter = NULL;    // ve sdilene pameti, pamatuje si kolikaty radek se tiskne do souboru
frontCounter_t * frontCounter = NULL;
int *customersWaiting = NULL;
bool *isPostOfficeOpen = NULL;

void print_message(char * format, ...);

bool handle_arguments(int argc, char * argv[], arguments_t * arguments);

bool init();

void clean_up();

void process_customer(int CustomerId, int maxTimeToWait);

void process_officials(int OfficialId, int maxTimeForBreak);

void process_postOffice(arguments_t * arguments);
