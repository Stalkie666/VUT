// Author: Jakub Hamadej, xhamad03@vutbr.cz, FIT VUT, 11/11/2024

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "packet_handler.hpp"



// recors capcured with pcap
std::shared_ptr<IsaTop> isaTop;

// pcap stuff
char errbuf[PCAP_ERRBUF_SIZE];
pcap_t * handle;

/**
 * Each second, print statistics and wipe data stored in them
 */
void next_second_alarm(int sig){
    // check if isaTop still exist
    if( isaTop != nullptr){
        isaTop->printRecords();
        isaTop->zeroRecords();
        isaTop->flagPrintRecords = true;
    }
        
    alarm(1);
}

/**
 * When want to end program, breakloop for pcap
 */
void terminal_alarm(int sig){
    if( handle ) pcap_breakloop(handle);
}

void printHelp(){
    std::cout << "Program isa-top" << std::endl;
    std::cout << "Jeho hlavnim ucelem je zachytavat provoz na siti" << std::endl;
    std::cout << "a vypisovat 10 nejaktivnejsich spojeni." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Uziti:" << std::endl;
    std::cout << "-i int    : Zadanim prepinace '-i int' zadate interface," << std::endl;
    std::cout << "            ze ktereho ma byt provoz zachytavan" << std::endl;
    std::cout << "-s b|p    : Zadanim prepinace '-s b|p' zvolite," << std::endl;
    std::cout << "            jestli ma byt statistika razena podle mnozstvi" << std::endl;
    std::cout << "            prenesenych dat (moznost 'b') nebo podle poctu" << std::endl;
    std::cout << "            prenesenych packetu (moznost 'p')" << std::endl;
    std::cout << "-h|--help : Vypise nabidku help" << std::endl;
    std::cout << std::endl;
    std::cout << "Pro ukonceni programu stisknete CTRL+C" << std::endl;
}

int main(int argc, char * argv[]){
    // arguments handler
    std::string interface = "";
    bool sortByBytes = true;
    bool isInterfaceSet = false;
    for(int i = 1; i < argc; ++i){
        if( strcmp("-i",argv[i]) == 0 && (i+1) < argc ){
            ++i;
            interface = argv[i];
            isInterfaceSet = true;
        }
        else if (strcmp("-s", argv[i]) == 0 && (i+1) < argc)
        {
            ++i;
            if( strcmp(argv[i],"b") == 0 )
                sortByBytes = true;
            else if( strcmp(argv[i],"p") == 0 )
                sortByBytes = false;
            else{
                std::cerr << "Neplatny argument pro sort, validne moznosti jsou pouze [-s b|p]" << std::endl;
                return 1;
            }
        }
        else if( strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0 ){
            printHelp();
            return 0;
        }
        else{
            std::cerr << "Neplatny argument, validne jsou pouze -i interface, [-s b|p]" << std::endl;
            return 1;
        }
        
    }

    if( isInterfaceSet == false ){
        std::cerr << "Nebyl zadan zadny interface, prosim zadejte -i interface, pripadne -h|--help" << std::endl;
        return 1;
    }
    
    handle = pcap_open_live(interface.c_str() ,BUFSIZ,1,1000,errbuf);
    if(!handle){
        isaTop = nullptr;
        std::cerr << "Na zadanem interface nebylo mozne poslouchat." << std::endl;
        return 1;
    }

    isaTop = std::make_shared<IsaTop>(sortByBytes);
    // set signals
        // ctrl + c
    signal(SIGINT,terminal_alarm);
        // every second alarm
    signal(SIGALRM,next_second_alarm);
    alarm(1);

    pcap_loop(handle,0,packet_handler,NULL);

    isaTop = nullptr;
    pcap_close(handle);
    return 0;
}