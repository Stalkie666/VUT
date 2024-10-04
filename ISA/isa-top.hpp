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
    private:
        // listenig Interface
        int listeningInterface;
        // TRUE if sort by bytes/s, FALSE if sort by packets/s
        bool sortByBytes;
        // store all records of captured packets
        std::vector<std::shared_ptr<Record>> records;

};