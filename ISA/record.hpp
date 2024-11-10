#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <sstream>


class Record{
    public:
        // Constructor
        Record( __uint128_t srcIP,  __uint128_t destIP, 
                uint32_t srcPort,   uint32_t destPort, 
                std::string protocolName, uint32_t transmitedBytes, bool isIPv4);
        // Destructor - will not be needed 
        // methods
            // return higher Number of bytes
        uint32_t numberOfBytes() const;
            // return higher Number of packets
        uint32_t numberOfPackets() const;
            // return string for print this record
        std::string printableRecords() const;
            // increase values
        void addBytes(const std::shared_ptr<Record> & data);
            // check if this is correct record or not
        bool isSameRecord(const std::shared_ptr<Record> & data) const;
    private:
        //private methods
        std::string ipv4ToString(__uint32_t IpAddress, uint16_t port) const;
        std::string ipv6ToString(__uint128_t IpAddress, uint16_t port) const;
        std::string bytesAndPacketsToRightFormat(uint32_t bytes, uint32_t packets) const;
        //types: IPv4 true, IPv6 false
        bool isIPv4;
        //src ip
        __uint128_t srcIp;
        //src port - if has one if not, then value is 0
        uint16_t srcPort;
        //dst ip
        __uint128_t destIp;
        //dst port - if has one, if not, then value is 0
        uint16_t destPort;
        // protocol name - store in string
        std::string protocolName;
        // some Rx - bytes/packets for SRC from DEST
        uint32_t recievedBytes;
        uint32_t recievedPackets;
        // some Tx - bytes/packets for DEST from SRC
        uint32_t transmitedBytes;
        uint32_t transmitedPackets;
};

#endif // __RECORD_HPP__