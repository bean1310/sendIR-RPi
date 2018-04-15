CC = gcc

sendIR2:	sendIR2.c
	$(CC) -o $@ -lwiringPi $@.c -Wall