CC=gcc
CFLAGS=-I.

process_memory: process_memory.c
	$(CC) -o code process_memory.c $(CFLAGS)

utils.o: utils.h
	$(CC) -o utils.o -c utils.c -pthread $(CFLAGS)

sum.o: sum.h
	$(CC) -o sum.o -c sum.c -pthread $(CFLAGS)

psum: sum.o utils.o sum.h utils.h 
	$(CC) -o psum sum.o utils.o parallel_sum.c -pthread $(CFLAGS)