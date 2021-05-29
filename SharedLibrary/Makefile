.PHONY: clean

LIB = random

CC = gcc

library: 
	$(CC) $(LIB).c -fPIC -c -o $(LIB).o
	$(CC) $(LIB).o -shared  -o lib$(LIB).so

client:
	$(CC) client.c -o client.o -l$(LIB) -L.
