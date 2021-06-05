.PHONY: clean

LIB = random

CC = gcc

library: 
	$(CC) $(LIB).c -fPIC -c -o $(LIB).o
	$(CC) $(LIB).o -shared  -o lib$(LIB).so

client:
	$(CC) client.c -l$(LIB) -L. -o client.o 

clean:
	rm $(LIB).o lib$(LIB).so
	rm client.o