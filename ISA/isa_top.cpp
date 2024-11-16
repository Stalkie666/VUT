// Author: Jakub Hamadej, xhamad03@vutbr.cz, FIT VUT, 11/11/2024

#include "isa_top.hpp"

IsaTop::IsaTop(bool sortByBytes){
    this->sortByBytes = sortByBytes;
    this->records.clear();
    this->flagPrintRecords = false;
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
        << std::left << std::setw(7) << "Proto"
        << std::left << std::setw(3) << "Rx:"
        << std::left << std::setw(8) << "B/s"
        << std::left << std::setw(8) << "P/s" 
        << std::left << std::setw(3) << "Tx:"
        << std::left << std::setw(8) << "B/s"
        << std::left << std::setw(8) << "P/s" 
        << std::endl;
    printw(ss.str().c_str());


    this->sortRecords();
    int N = (this->records.size() >= 10 ) ? 10 : this->records.size();
    for(int i = 0; i < N; ++i)
        if( this->records[i]->numberOfPackets() != 0 ) printw("%s\n", this->records[i]->printableRecords().c_str());
    refresh();
    
    return 0;
}

/**
 * set all records to 0
 */
void IsaTop::zeroRecords(){
    for(auto record : this->records){
        record->zeroRecord();
    }
}