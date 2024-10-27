#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "packet_handler.hpp"


// recors capcured with pcap
std::shared_ptr<IsaTop> isaTop;
char errbuf[PCAP_ERRBUF_SIZE];
pcap_t * handle;

// flag for keeping program running
bool keepRunning = true;

/**
 * Each second, print statistics and wipe data stored in them
 */
void next_second_alarm(int sig){
    // check if isaTop still exist
    if( isaTop != nullptr)
        isaTop->printRecords();
    alarm(1);
}

/**
 * When want to end program, send terminating signal
 */
void terminal_alarm(int sig){
    if( handle ) pcap_breakloop(handle);
    keepRunning = false;
}


int main(int argc, char * argv[]){
    
    
    // TODO: zpracovani argumentu

    isaTop = std::make_shared<IsaTop>(5,false);
    
    handle = pcap_open_live("wlp2s0",BUFSIZ,1,1000,errbuf);
    if(!handle){
        isaTop = nullptr;
        fprintf(stderr,"Neslo to otevrit.\n");
        return 0;
    }

    // set signals
        // ctrl + c
    signal(SIGINT,terminal_alarm);
        // every second alarm
    signal(SIGALRM,next_second_alarm);
    alarm(1);

    pcap_loop(handle,0,packet_handler,NULL);

    isaTop = nullptr;
    printf("Konec\n");
    return 0;
}