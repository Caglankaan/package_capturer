#include "network_packages.h"

struct sockaddr_in source,dest;

void NetworkPackages::send_to_regex(const std::string &str)
{
    if(NetworkPackages::is_regex_defined)
    {
        if(regex_search(str, NetworkPackages::reg))
        {
            std::cout << str << "\n";
        }
    }
    else
    {
        std::cout << str << "\n";
    }
    
    
    NetworkPackages::pool->erase();
    
}

//https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
std::string get_data_payload (const u_char * data , int Size)
{
    std::stringstream ss;
	int i , j;
	for(i=0 ; i < Size ; i++)
	{
		if( i!=0 && i%16==0)   //if one line of hex printing is complete...
		{
			ss << "         ";
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					ss << (unsigned char)data[j]; //if its a number or alphabet
				
				else ss <<"."; //otherwise print a dot
			}
            ss << "\n";
		} 
		
		if(i%16==0) ss << "   ";
			ss << (unsigned int)data[i];
				
		if( i==Size-1)  //print the last spaces
		{
			for(j=0;j<15-i%16;j++) 
			{
			  ss << "   "; //extra spaces
			}
			
			ss << "         ";
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) 
				{
				  ss << (unsigned char)data[j];
				}
				else 
				{
				  ss << ".";
				}
			}
			
			ss << "\n" ;
		}
	}
    return ss.str();
}

void NetworkPackages::packetHandler( u_char *args, const struct pcap_pkthdr* header,  const u_char* buffer) 
{
    
	int size = header->len;
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));

	iphdrlen = iph->ihl*4;
	
	struct tcphdr *tcph=(struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	struct ethhdr *eth = (struct ethhdr *)buffer;
			
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;

	int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
    std::stringstream ss;
    
    char dest_address[18];
    char source_address[18];
    int ret_val;
    ret_val = snprintf(dest_address, 18, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5]);
    ret_val = snprintf(source_address, 18, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5]);
    
    ss << "--------------------------------------------------------\n";
    ss << "Destionation Address    : " << dest_address << "\n";
    ss << "Source Address          : " << source_address << "\n";
    ss << "Source IP               : " << inet_ntoa(source.sin_addr) << "\n";
    ss << "Destination IP          : " << inet_ntoa(dest.sin_addr) << "\n";
    ss << "Protocol                : " << (unsigned short)eth->h_proto << "\n";
    ss << "Source Port             : " << ntohs(tcph->source) << "\n";
    ss << "Destination Port        : " << ntohs(tcph->dest) << "\n";
    ss << "Data Payload            :\n" << get_data_payload(buffer+header_size, (size - header_size)) << "\n";
    ss << "--------------------------------------------------------\n";

    std::string packet_info = ss.str();
    NetworkPackages::pool->add(send_to_regex, ss.str());
    
    
}

void NetworkPackages::start() {
    
    char *dev;
    pcap_t *descr;
    char errbuf[PCAP_ERRBUF_SIZE];

    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        std::cout << "pcap_lookupdev() failed: " << errbuf << "\n";
        exit(1);
    }

    descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (descr == NULL) {
        std::cout << "pcap_open_live() failed: " << errbuf << "\n";
        exit(1);
    }

        
    if (pcap_loop(descr, 0, packetHandler, NULL) < 0) {
        std::cout << "pcap_loop() failed: " << pcap_geterr(descr);
        exit(1);
    }

    std::cout << "capture finished" << "\n";

}