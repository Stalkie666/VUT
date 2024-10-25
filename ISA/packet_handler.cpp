#include "packet_handler.hpp"

// recors capcured with pcap
std::shared_ptr<IsaTop> isaTop;
char errbuf[PCAP_ERRBUF_SIZE];
pcap_t * handle;

void packet_handler(u_char *user, const struct pcap_pkthdr * header, const u_char *packet){
    // TODO: zde zpracovani packetu
    isaTop->test++;
}