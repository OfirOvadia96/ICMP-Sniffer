# ICMP And Sniffer


<h2>Part I:</h2>

#### In this section I took a code that was given to me and modified it so that it would be able to send ICMP packages and create a program called myping.cpp program ####
* That is Will send: ICMP ECHO REQUEST And ICMP-ECHO-REPLY

* in addition: calculates the RTT time in milliseconds and microseconds.

* This program will send ICMP to 8.8.8.8 (Google)

* To complie Using MAKEFILE I built and then to run you have to use SUDO in the run command line 

* In addition there is wireshark PCAP documentation


*(And of course here we will connect to the second part with which "SNIFFING" these packages)*


<h2>Part II:</h2>

#### Build a listener for ICMP network packages (sniffer) ####

The tool will:
* sniffing ICMP traffic on the network and 
* display IP_SRC, IP_DST, TYPE, CODE on the screen 


*(Note that the card's ability to see traffic not ours depends on the type of connection and the type of network card we need to transfer to monitor mode)*

To compile Using MAKEFILE I built and then to run you have to use SUDO in the run command line 
