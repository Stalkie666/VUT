// Author: Jakub Hamadej, xhamad03@vutbr.cz, FIT VUT, 11/11/2024

#include "isa_top.hpp"

IsaTop::IsaTop(bool sortByBytes){
    this->sortByBytes = sortByBytes;
    this->records.clear();
    initscr();
    printw("Nacitani dat...");
    refresh();
}

IsaTop::~IsaTop(){
    this->records.clear();
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
    if( this->sortByBytes )
        std::sort(this->records.begin(),this->records.end(), [](const std::shared_ptr<Record> & a,const std::shared_ptr<Record> & b){ return a->numberOfBytes() > b->numberOfBytes(); });
    else
        std::sort(this->records.begin(),this->records.end(), [](const std::shared_ptr<Record> & a,const std::shared_ptr<Record> & b){ return a->numberOfPackets() > b->numberOfPackets(); });
    return 0;
}

int IsaTop::deleteAllRecords(){
    this->records.clear();
    return 0;
}

int IsaTop::printRecords(){
    clear();
    std::stringstream ss;
    ss  << std::left << std::setw(50) << "Src IP:port"
        << std::left << std::setw(50) << "Dst IP:port"
        << std::left << std::setw(10) << "Proto"
        << std::left << std::setw(10) << "Rx" 
        << std::left << std::setw(10) << "Tx" << std::endl;
    printw(ss.str().c_str());


    this->sortRecords();
    int N = (this->records.size() >= 10 ) ? 10 : this->records.size();
    for(int i = 0; i < N; ++i)
        printw("%s\n", this->records[i]->printableRecords().c_str());
    refresh();
    // clear records for next second
    this->deleteAllRecords();
    
    return 0;
}