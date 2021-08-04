// icmp.cpp
// Sending ICMP Echo Requests using Raw-sockets.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h> // gettimeofday()

 // IPv4 header len without options
#define IP4_HDRLEN 20

// ICMP header len for echo req
#define ICMP_HDRLEN 8 

// Checksum algo
unsigned short calculate_checksum(unsigned short * paddress, int len);


#define SOURCE_IP "10.0.2.15"
// i.e the gateway or ping to google.com for their ip-address
#define DESTINATION_IP "8.8.8.8"
#define PACKETSIZE 64

struct packet {
    struct icmphdr icmphdr; // ICMP-header
    char data[PACKETSIZE - sizeof(icmphdr)] = "This is the ping.\n";
};


int main (){
	struct timeval start,end;
    struct packet mypacket;

    //===================
    // ICMP header
    //===================

    // Message Type (8 bits): ICMP_ECHO_REQUEST
    mypacket.icmphdr.type = ICMP_ECHO;

    // Message Code (8 bits): echo request
    mypacket.icmphdr.code = 0;

    // Identifier (16 bits): some number to trace the response.
    // It will be copied to the response packet and used to map response to the request sent earlier.
    // Thus, it serves as a Transaction-ID when we need to make "ping"
    mypacket.icmphdr.un.echo.id = 18; // hai

    // Sequence Number (16 bits): starts at 0
    mypacket.icmphdr.un.echo.sequence = 0;

    // ICMP header checksum (16 bits): set to 0 not to include into checksum calculation
    mypacket.icmphdr.checksum = 0;

    // Calculate the ICMP header checksum
    mypacket.icmphdr.checksum = calculate_checksum((unsigned short *) &mypacket, sizeof(mypacket));
 
    //AF_INET - for IPV4
    //inet_addr - converts the address from IPv4 (numbers-and-dots) into binary data
    // DESTINATION_IP - google ipAddress
    //dest_in - the destination
    struct sockaddr_in dest_in;
    dest_in.sin_family = AF_INET;
	dest_in.sin_addr.s_addr = inet_addr(DESTINATION_IP);


    // Create raw socket for IP-RAW (make IP-header by yourself)
    //SOCK_RAW - type socket
    //AF_INET - for IPV4
    //IPPROTO_ICMP - for echo icmp message (type message)
    int sock = -1;
    if ((sock = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        fprintf (stderr, "socket() failed with error: %d",errno);
        fprintf (stderr, "To create a raw socket, the process needs to be run by Admin/root user.\n\n");
        return -1;
    }

    // This socket option IP_HDRINCL says that we are building IPv4 header by ourselves, and
    // the networking in kernel is in charge only for Ethernet header.
    //

	gettimeofday(&start, NULL);
    // Send the packet using sendto() for sending datagrams.
    if (sendto (sock, &mypacket, sizeof(mypacket), 0, (struct sockaddr *) &dest_in, sizeof (dest_in)) == -1)  {
        fprintf (stderr, "sendto() failed with error: %d",errno);
        return -1;
    }

	int n = 0;
    //r_addr - gets the replay from the destination
	struct sockaddr_in r_addr;
	socklen_t len = sizeof(r_addr); // size
	while (n <=0){
        //recive the replay icmp
		n = recvfrom(sock, &mypacket, sizeof(mypacket), 0, (struct sockaddr *)&r_addr, &len);
		gettimeofday(&end, NULL);
	}

    //calculate RTT:
	int seconds = end.tv_sec - start.tv_sec;
	unsigned int microseconds = end.tv_usec - start.tv_usec;
	unsigned int milliseconds = (microseconds / 1000);
	printf("Time in second is : %d \n", seconds);
	printf("Time in milliseconds is: %u \n", milliseconds);
	printf("Time in microseconds is : %u \n", 	microseconds);

  // Close the raw socket descriptor.
  close(sock);

  return 0;
}

// Compute checksum
unsigned short calculate_checksum(unsigned short * paddress, int len){
	int nleft = len;
	int sum = 0;
	unsigned short * w = paddress;
	unsigned short answer = 0;

	while (nleft > 1){
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1){
		*((unsigned char *)&answer) = *((unsigned char *)w);
		sum += answer;
	}

	// add back carry outs from top 16 bits to low 16 bits
	sum = (sum >> 16) + (sum & 0xffff); // add hi 16 to low 16
	sum += (sum >> 16);                 // add carry
	answer = ~sum;                      // truncate to 16 bits

	return answer;
}



