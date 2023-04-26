#include <stdbool.h>

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
    int time
}official_t;

// ve sdilene pameti, pamatuje si kolikaty radek se tiskne do souboru
int * numberOfLines;
bool * isPostOfficeOpen;

bool handle_arguments(int argc, char * argv[], arguments_t * arguments);

bool init();

void process_customer();

void process_officials();

void process_postOffice();

void clean_up();