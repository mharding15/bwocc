#Makefile for “bwocc” C++ CMSC858D - HW1
#Created by Matt Harding 10/22/20

PROG = bwocc
CC = g++
CPPFLAGS = -g --std=c++11
OBJS = main.o bit_vector.o rank_support.o select_support.o utils.o

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS)
main.o :
	$(CC) $(CPPFLAGS) -c main.cpp
bit_vector.o : bit_vector.h
	$(CC) $(CPPFLAGS) -c bit_vector.cpp
rank_support.o: rank_support.h
	$(CC) $(CPPFLAGS) -c rank_support.cpp
select_support.o: select_support.h
	$(CC) $(CPPFLAGS) -c select_support.cpp
utils.o: utils.h
	$(CC) $(CPPFLAGS) -c utils.cpp
clean:
	rm -f core $(PROG) $(OBJS)
