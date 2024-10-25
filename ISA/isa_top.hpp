#ifndef __ISA_TOP_HPP__
#define __ISA_TOP_HPP__


#include <iostream>
#include <vector>
#include <memory>
#include <ncurses.h>
#include <pcap/pcap.h>

class Record{
    public:
    private:
        //src ip
        //src port
        //dst ip
        //dst port
        // protocol name
        // some Rx shit
        // some Tx shit
};



class IsaTop{
    public:
        IsaTop(int listeningInterface,bool sortByBytes);
        ~IsaTop();
        int addRecord(std::shared_ptr<Record>);
        int sortRecords();
        int deleteAllRecords();
        int printRecords();

        int test = 0;
    private:
        // listenig Interface
        int listeningInterface;
        // TRUE if sort by bytes/s, FALSE if sort by packets/s
        bool sortByBytes;
        // store all records of captured packets
        std::vector<std::shared_ptr<Record>> records;

};



#endif //__ISA_TOP_HPP__