#include "record.hpp"

Record::Record( __uint128_t srcIP,  __uint128_t destIP, 
                uint32_t srcPort,   uint32_t destPort, 
                std::string protocolName, uint32_t transmitedBytes, bool isIPv4){
                    // set up differently
                    this->srcIp = srcIP;
                    this->destIp = destIP;
                    this->srcPort = srcPort;
                    this->destPort = destPort;
                    this->protocolName = protocolName;
                    this->transmitedBytes = transmitedBytes;
                    this->isIPv4 = isIPv4;
                    // default set up
                    this->transmitedPackets = 1;
                    this->recievedBytes = 0;
                    this->recievedPackets = 0;
                }


uint32_t Record::numberOfBytes() const{
    return (this->transmitedBytes > this->recievedBytes) ? this->transmitedBytes : this->recievedBytes;
}

uint32_t Record::numberOfPackets() const{
    return (this->transmitedPackets > this->recievedPackets) ? this->transmitedPackets : this->recievedPackets;
}

std::string Record::printableRecords() const{
    // TODO: zpracovat hodnoty pro tisk do stringu, 
    // upravit Bajty a Packety na prijatelne hodnoty
    // a spravne zaokrouhlit

    return "TODO: hodnoty na tisk neudelany xd";
}

void Record::addBytes(const std::shared_ptr<Record> & data){
    if  (   this->srcIp == data->srcIp && this->srcPort == data->srcPort && 
            this->destIp == data->destIp && this->destPort == data->destPort){
                this->transmitedBytes += data->transmitedBytes;
                this->transmitedPackets++;
        }
    else{
        this->recievedBytes += this->transmitedBytes;
        this->recievedPackets++;
    }
}

bool Record::isSameRecord(const std::shared_ptr<Record> & data) const{
    if  (   this->srcIp == data->srcIp && this->srcPort == data->srcPort && 
            this->destIp == data->destIp && this->destPort == data->destPort &&
            this->protocolName.compare(data->protocolName) == 0
        ) 
        return true;
    else if (   this->srcIp == data->destIp && this->srcPort == data->destPort && 
                this->destIp == data->srcIp && this->destPort == data->srcPort &&
                this->protocolName.compare(data->protocolName) == 0
            ) 
            return true;
    else return false;
}
