cc = gcc
cflags = -lpthread
dpflags = -lpthread -lncurses

all:
	$(cc) -o dining_philosophers dining_philosophers.c $(dpflags) 
	$(cc) -o produtor_consumidor produtor_consumidor.c $(cflags) 