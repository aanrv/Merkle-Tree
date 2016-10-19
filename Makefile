CC = g++
FLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic
LIBS = -lcryptopp -lpthread

default: a.out

a.out:	main.o
	$(CC) $(FLAGS) -o a.out main.o $(LIBS)

main.o:	main.cpp
	$(CC) $(FLAGS) -c -o main.o main.cpp $(LIBS)

clean:
	rm -f *.o *.out

