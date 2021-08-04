CC = g++


all: myping sniffer

myping: myping.o
	$(CC) myping.o -o myping

sniffer: sniffer.o
	gcc sniffer.o -o sniffer

myping.o: myping.cpp
	$(CC) -c myping.cpp

sniffer.o: sniffer.c
	gcc -c sniffer.c

.PHONY: clean all

clean: 
	rm -f *.o myping sniffer
