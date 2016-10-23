CC = g++
FLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic
LIBS = -lcryptopp -lpthread

default: a.out

a.out:	main.o merkletree.o merklenode.o
	$(CC) $(FLAGS) -o a.out main.o merkletree.o merklenode.o $(LIBS)

main.o:	main.cpp
	$(CC) $(FLAGS) -c -o main.o main.cpp

merkletree.o:	merkletree.cpp
	$(CC) $(FLAGS) -c -o merkletree.o merkletree.cpp

merklenode.o:	merklenode.cpp
	$(CC) $(FLAGS) -c -o merklenode.o merklenode.cpp

clean:
	rm -f *.o *.out

