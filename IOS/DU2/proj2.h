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

bool handle_arguments(int argc, char * argv[], arguments_t * arguments);

void wait_random_time();

void process_customer();

void take_a_break();

void process_officials();

void process_postOffice();