#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <semaphore.h>
#include <sys/mman.h>

typedef struct arguments{
    int numberOfCustomers;
    int numberOfOfficials;
    int maxTimeWaitCustomer;
    int maxTimeOfficialsBreak;
    int maxTimePostOfficeIsClosed;
}arguments_t;

enum customerTask{
    letters = 1,
    packages = 2,
    financialServices = 3
};

typedef struct customer{
    int idZ;
    int chossenActivity;
    bool isInFront;
}customer_t;

typedef struct official{
    int idU;
    int time;
}official_t;

//semafory
sem_t * s_write;
sem_t * stask1;
sem_t * stask2;
sem_t * stask3;


FILE * streamFile = NULL;
int * lineCounter;    // ve sdilene pameti, pamatuje si kolikaty radek se tiskne do souboru
bool * isPostOfficeOpen;

void print_message(char * format, ...);

bool handle_arguments(int argc, char * argv[], arguments_t * arguments);

bool init();

void process_customer(int CustomerId);

void process_officials(int OfficialId);

void process_postOffice();

void clean_up();