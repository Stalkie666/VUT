#include "packet_handler.hpp"

// size of ling layer
#define IPvX_OFFSET  14
// link layer (14) + size of IPv6 header(40)
#define IPv6_DATA_OFFSET 54

void packet_handler(u_char *user, const struct pcap_pkthdr * header, const u_char *packet){
    std::shared_ptr<Record> record  = nullptr;

    uint8_t IPvX = *(packet + IPvX_OFFSET) & 0xF0;

    // IPv4
    if(IPvX == 0x40 ){
        // offset of protocol in IPv4 packet is 23
        uint8_t Protocol = *(packet + (IPvX_OFFSET + 9));
        // data offset starts at IHL * 4bytes, minimal IHL is 5, options make it longer
        unsigned dataOffset = ((unsigned)( *(packet + IPvX_OFFSET) & 0x0F)) * 4;
        // ip addreses from IPv4 header packet
        uint32_t srcIp = *((uint32_t*)(packet + (IPvX_OFFSET + 12)) );
        uint32_t dstIp = *((uint32_t*)(packet + (IPvX_OFFSET + 16)) );

        if( Protocol == 6 /*TCP*/){
            uint16_t srcPort = *((uint16_t*)(packet + (IPvX_OFFSET + dataOffset) ));
            uint16_t dstPort = *((uint16_t*)(packet + (IPvX_OFFSET + dataOffset + 2)));
            record = std::make_shared<Record>(srcIp, dstIp, srcPort, dstPort, "tcp", header->len, true );
        }
        else if( Protocol == 17 /*UDP*/){
            uint16_t srcPort = *((uint16_t*)(packet + (IPvX_OFFSET + dataOffset) ));
            uint16_t dstPort = *((uint16_t*)(packet + (IPvX_OFFSET + dataOffset + 2)));
            record = std::make_shared<Record>(srcIp, dstIp, srcPort, dstPort, "udp", header->len, true );
        }
        else if( Protocol == 1 /*ICMP*/){
            record = std::make_shared<Record>( (__uint128_t)srcIp, (__uint128_t)dstIp, 0, 0, "icmp", header->len, true );
        }
        else return;
    }
    // IPv6
    else if( IPvX == 0x60){
        // offset of next header in IPv6 packet is 20
        uint8_t Protocol = *(packet + (IPvX_OFFSET + 6));
        // ip addreses from IPv6 header packet
        __uint128_t srcIp = *((__uint128_t*)(packet + (IPvX_OFFSET + 8)));
        __uint128_t dstIp = *((__uint128_t*)(packet + (IPvX_OFFSET + 24)));

        if( Protocol == 6 /*TCP*/){
            uint16_t srcPort = *((uint16_t*)(packet + (IPv6_DATA_OFFSET)));
            uint16_t dstPort = *((uint16_t*)(packet + (IPv6_DATA_OFFSET + 2)));
            record = std::make_shared<Record>(srcIp, dstIp, srcPort, dstPort, "tcp", header->len, false );
        }
        else if( Protocol == 17 /*UDP*/){
            uint16_t srcPort = *((uint16_t*)(packet + (IPv6_DATA_OFFSET)));
            uint16_t dstPort = *((uint16_t*)(packet + (IPv6_DATA_OFFSET + 2)));
            record = std::make_shared<Record>(srcIp, dstIp, srcPort, dstPort, "udp", header->len, false );
        }
        else if( Protocol == 58 /*ICMPv6*/){
            record = std::make_shared<Record>(srcIp, dstIp, 0, 0, "icmp6", header->len, false );
        }
        else return;
    }
    else{
        // do nothing or just for debug, counting else, who knows
        return;
        
    }
    isaTop->addRecord(record);
}