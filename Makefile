cc = gcc
cflags = -lpthread

all:
	$(cc) -o dining_philosophers dining_philosophers.c $(cflags) 
	$(cc) -o produtor_consumidor produtor_consumidor.c $(cflags) 