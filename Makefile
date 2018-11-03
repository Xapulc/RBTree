CC = g++ $(CFLAGS)
CFLAGS = -g -O0 -Wall

all: treeTst

treeTst: treeTst.o RBTree.o
	$(CC) -o treeTst treeTst.o RBTree.o

treeTst.o: treeTst.cpp RBTree.h
	$(CC) -c treeTst.cpp

RBTree.o: RBTree.cpp RBTree.h
	$(CC) -c RBTree.cpp

clean:
	rm -f *.o treeTst tmp*
