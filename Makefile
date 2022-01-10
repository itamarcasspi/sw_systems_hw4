CC=gcc
FLAGS= -Wall 

all: graph

graph: main.o graph.a 
	$(CC) $(FLAGS) -o graph main.o graph.a 

graph.a: graph.o
	ar rcs graph.a graph.o

graph.o: graph.c graph.h
	$(CC) $(FLAGS) -c graph.c

main.o: main.c graph.h
	$(CC) $(FLAGS) -c main.c

.PHONY: clean

clean: 
	rm -rf *.o *.a graph

