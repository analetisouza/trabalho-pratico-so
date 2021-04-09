cc = gcc
cflags = -lpthread

all:
	$(cc) -o dp dining_philosophers.c $(cflags) 
	$(cc) -o pc produtor_consumidor.c $(cflags) 
	$(cc) -o edp experimental_dining_philosophers.c $(cflags) 
