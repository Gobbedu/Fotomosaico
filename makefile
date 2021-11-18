# =======================================================
#   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
#   GRR20203892
#   Makefile do programa FOTOMOSAICO
#  ======================================================= 

CFLAGS = -Wall -ansi -g -std=c99
LDLIBS = -lm

# nome do arquivo compilado
NAME = mosaico

# codigo
CODED = main.c utils.c ppm.c 
HEADERS = utils.h ppm.h 
#objetos gerados
objects = main.o utils.o ppm.o 


# REGRAS DE COMPILACAO
all: main

main: $(objects) 
	gcc -o $(NAME) $(objects) $(LDLIBS)

main.o: $(CODED)
	gcc $(CFLAGS) -c $(CODED)

clean:
	rm $(objects) 

purge: clean
	rm $(NAME)
