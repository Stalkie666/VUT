#ifndef __PACKET_HANDLER_HPP_
#define __PACKET_HANDLER_HPP_

#include "isa-top.hpp"



void packet_handler(u_char *user, const struct pcap_pkthdr * header, const u_char *packet);

#endif //__PACKET_HANDLER_HPP_