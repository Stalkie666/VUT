#include "isa-top.hpp"

IsaTop::IsaTop(int listeningInterface,bool sortByBytes){
    this->sortByBytes = sortByBytes;
    this->listeningInterface = listeningInterface;
    this->records.clear();
    initscr();
    printw("Inicialized");
    refresh();
}

IsaTop::~IsaTop(){
    endwin();
}

int IsaTop::addRecord(std::shared_ptr<Record>){
    // TODO: pridani zaznamu
    // zjistit jestli jiz zaznam existuje
        // pokud ano, pridat packety
        // pokud ne, vytvorit novy zaznam
    return 0;
}

int IsaTop::sortRecords(){
    // TODO: radit zaznamy
        // podle poctu packetu
        // podle poctu bytu
    return 0;
}

int IsaTop::deleteAllRecords(){
    // TODO:
    this->records.clear();
    return 0;
}
int IsaTop::printRecords(){
    clear();
    printw("Src IP:port\t\tDst Ip:port\t\tProto\tRx\tTx\n");
    // TODO: dodelat tisknuti jednotlivych zaznamu
    printw("%d", this->test);
    refresh();
    return 0;
}