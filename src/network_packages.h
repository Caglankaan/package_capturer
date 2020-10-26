#pragma once
#ifndef NETWORK_PACKAGES_HEADER
#define NETWORK_PACKAGES_HEADER

#include <iostream>
#include <pcap.h>
#include <regex>
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string.h> //for memset
#include <sstream>
#include <fstream>
#include "thread_pool.h"

#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header

namespace NetworkPackages
{
    extern ThreadPool *pool;
    extern std::regex reg;
    extern bool is_regex_defined;
    extern FILE *file;
    
    /*
    struct NetworkPackage
    {
        NetworkPackage(std::string &source_ip_, std::string &destination_ip_, std::string &source_address_, 
                    std::string &destination_address_, unsigned short protocol_, std::string &data_payload_, 
                    uint16_t source_port_, uint16_t destination_port):protocol(protocol_) {}

        unsigned short protocol;
        std::string source_ip;
        std::string destination_ip;
        std::string source_address;
        std::string destination_address;
        std::string packet_itself;
        std::string ip_header;
        std::string tcp_header;
        std::string data_payload;
        ~NetworkPackage(){}
    };*/
   

    void packetHandler(u_char *userData, const struct pcap_pkthdr* header,  const u_char* buffer);
    void send_to_regex(const std::string &str);
    void start() ;
}


#endif