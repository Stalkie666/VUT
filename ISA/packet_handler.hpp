#ifndef __PACKET_HANDLER_HPP_
#define __PACKET_HANDLER_HPP_

#include <string.h>
#include "isa_top.hpp"

// recors capcured with pcap
extern std::shared_ptr<IsaTop> isaTop;
extern char errbuf[PCAP_ERRBUF_SIZE];
extern pcap_t * handle;


void packet_handler(u_char *user, const struct pcap_pkthdr * header, const u_char *packet);

#endif //__PACKET_HANDLER_HPP_