#include "record.hpp"

Record::Record( __uint128_t srcIP,  __uint128_t destIP, 
                uint32_t srcPort,   uint32_t destPort, 
                std::string protocolName, uint32_t transmitedBytes, bool isIPv4){
                    // set up differently
                    this->srcIp = srcIP;
                    this->destIp = destIP;
                    // input was in big endian here is little endian
                    this->srcPort  = ((srcPort  & 0xff00) >> 8) + ((srcPort  & 0x00ff) << 8);
                    this->destPort = ((destPort & 0xff00) >> 8) + ((destPort & 0x00ff) << 8);

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
    std::stringstream ss;
    if( this->isIPv4 ){
         ss << this->ipv4ToString(this->srcIp,this->srcPort);
         ss << "\t";
         ss << this->ipv4ToString(this->destIp,this->destPort);
         ss << "\t" << this->protocolName;
         ss << "\t" << this->bytesAndPacketsToRightFormat(this->recievedBytes,this->recievedPackets);
         ss << "\t" << this->bytesAndPacketsToRightFormat(this->transmitedBytes,this->transmitedPackets);
    }
    else{
        ss << this->ipv6ToString(this->srcIp,this->srcPort);
        ss << "\t";
        ss << this->ipv6ToString(this->destIp,this->destPort);
        ss << "\t" << this->protocolName;
        ss << "\t" << this->bytesAndPacketsToRightFormat(this->recievedBytes,this->recievedPackets);
        ss << "\t" << this->bytesAndPacketsToRightFormat(this->transmitedBytes,this->transmitedPackets);
    }

    return ss.str();
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


/**
 * @return string of parsed Ip address and it's port, if had any
 */
std::string Record::ipv4ToString(__uint32_t IpAddress, uint16_t port) const{
    std::stringstream ss;
    uint32_t mask = 0xff;
    // srcIp
    for( int i = 0; i < 4; i++ ){
        ss << ((IpAddress & mask) >> (i*8) );
        if( i < 3 ) ss << ".";
        mask = mask << 8;
    }
    // if it is not icmp
    if( port != 0 ){
        ss << ":" << port;
    }
    return ss.str();
}
/**
 * @return string of parsed Ip address and it's port, if had any
 */
std::string Record::ipv6ToString(__uint128_t IpAddress, uint16_t port) const{
    // careful, ipaddress is in Bigendian - need to start with mask from start to print, port is in little endian
    std::stringstream ss;
    __uint128_t mask = 0xffff;
    ss << "[";
    for(int i = 0; i < 8; ++i){
        ss << std::hex << (uint32_t)((IpAddress & mask) >> (i*32));
        if(i != 7) ss << ":";
        mask = mask << 32;
    }
    ss << "]";
    // if it is not icmp6
    if( port != 0 ){
        ss << ":" << std::dec << port;
    }
    return ss.str();
}


/**
 * @return string of parsed bytes and packets
 */
std::string Record::bytesAndPacketsToRightFormat(uint32_t bytes, uint32_t packets) const{
    std::stringstream ss;
    // >= million bytes
    double tmpBytes;
    if(bytes >= 1000000){
        tmpBytes = bytes;
        tmpBytes = tmpBytes / 1000000;
        ss << tmpBytes << "M";
    }
    // >= thousands bytes
    else if(bytes >= 1000){
        tmpBytes = bytes;
        tmpBytes = tmpBytes / 1000;
        ss << tmpBytes << "k";
    }
    // < thousand bytes
    else{
        ss << bytes;
    }
    ss << " ";

    double tmpPackets;
    // >= million packets
    if(packets >= 1000000){
        tmpPackets = packets;
        tmpPackets = tmpPackets / 1000000;
        ss << tmpPackets << "M";
    }
    // >= thousands packets
    else if(packets >= 1000){
        tmpPackets = packets;
        tmpPackets = tmpPackets / 1000000;
        ss << tmpPackets << "k";
    }
    // < thousand packets
    else{
        ss << packets;
    }
    return ss.str();
}