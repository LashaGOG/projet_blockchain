CC = gcc
CFLAGS = -g -Wall

all : main

main : main.o rw_data.o
	$(CC) $(CFLAGS) -o $@ $^ 
main.o : main.c
	$(CC) $(CFLAGS) -o $@ $^ -c 


clean : 
	rm *.o 
