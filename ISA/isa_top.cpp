#include "isa_top.hpp"

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
/**
 * add bytes to correct record or push_back new record into vector
 * @return 0 if success, 1 if iput is nullptr
 */
int IsaTop::addRecord(std::shared_ptr<Record> & addingRecord){
    if( addingRecord == nullptr ) return 1;
    bool addNewRecord = true;
    for( auto currentRecord : this->records ){
        if( currentRecord->isSameRecord(addingRecord) ){
            currentRecord->addBytes(addingRecord);
            addNewRecord = false;
            break;
        }
    }
    if(addNewRecord) 
        this->records.push_back(addingRecord);
    return 0;
}

int IsaTop::sortRecords(){
    // TODO: radit zaznamy
        // podle poctu packetu
        // podle poctu bytu
    return 0;
}

int IsaTop::deleteAllRecords(){
    this->records.clear();
    return 0;
}

int IsaTop::printRecords(){
    clear();
    printw("Src IP:port\t\tDst Ip:port\t\tProto\tRx\tTx\n");
    
    /* TODO: SMAZAT, jen na debug */ printw("Records=%ld\n", this->records.size());

    this->sortRecords();
    int N = (this->records.size() >= 10 ) ? 10 : this->records.size();
    for(int i = 0; i < N; ++i)
        printw("%d: %s\n",(i+1), this->records[i]->printableRecords().c_str());
    refresh();
    // clear records for next second
    this->deleteAllRecords();
    
    return 0;
}