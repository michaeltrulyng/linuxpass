# makefile for various utility programs
# make -B will force compile (even if file is up-to-date)
CC = gcc
CFLAGS = -g -Wall

default: compile

compile: filefix

encrypt: encrypt.o
	$(CC) $(CFLAGS) -o encrypt encrypt.o -l crypt

encrypt.o: encrypt.c
	@echo compiling filefix
	$(CC) $(CFLAGS) -c encrypt.c

filefix: filefix.o
	$(CC) $(CFLAGS) -o filefix filefix.o

filefix.o: filefix.c filelib.h
	@echo compiling filefix
	$(CC) $(CFLAGS) -c filefix.c

clean:
	$(RM) filefix *.o *~

run:
	./filefix.sh 
