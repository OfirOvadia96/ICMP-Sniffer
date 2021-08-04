#include<stdio.h>	//For standard things
#include<stdlib.h>	//malloc
#include<string.h>	//memset
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header
#include<sys/socket.h>
#include<arpa/inet.h>


void print_ip_header(unsigned char* , int);
void print_icmp_packet(unsigned char* , int);

struct sockaddr_in source,dest; // for source and dest ip


int main(){
    int sock_raw;
	int saddr_size;
	struct sockaddr saddr;
	struct in_addr in;
	
	//malloc for big memory - more then we need
	unsigned char *buffer = (unsigned char *)malloc(65536);
	printf("Start sniffing...\n");
    printf("\n");

	//Create a raw socket that shall sniff
	sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_ICMP);
	if(sock_raw < 0){
		printf("Socket Error\n");
		return 1;
	}

    int data_size = 1;
	while(data_size){
		saddr_size = sizeof saddr;
		//Receive a packet
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("failed to get packets\n");
			return 1;
		}
		//print details
        print_ip_header(buffer,data_size);
        print_icmp_packet(buffer,data_size);
        
	}
	close(sock_raw); //close the socket
	return 0;
}

void print_ip_header(unsigned char* Buffer, int Size){
	unsigned short iphdrlen;
		
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen =iph->ihl*4;

	//memset - Sets the first num bytes of the block of memory pointed by ptr to the specified value 
	//init the source
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	//init the dest
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;
	printf("Source IP: %s \n",inet_ntoa(source.sin_addr));
	printf("Destination IP: %s \n",inet_ntoa(dest.sin_addr));
}


void print_icmp_packet(unsigned char* Buffer , int Size){
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;
	
	struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen);	
	
	printf("Type : %d \n",(unsigned int)(icmph->type));
	printf("Code : %d \n",(unsigned int)(icmph->code));
}