CC = gcc
CFLAGS = -Wall
SIZE = 1000

all: H03.out

H03.out: thompsonH03.o vector.o fraction.o
	$(CC) $(CFLAGS) thompsonH03.o vector.o fraction.o -o H03.out

thompsonH03.o: thompsonH03.c
	$(CC) $(CFLAGS) -c thompsonH03.c

vector.o: vector.c
	$(CC) $(CFLAGS) -c vector.c

fraction.o: fraction.c
	$(CC) $(CFLAGS) -c fraction.c

fractions:
	python generateFractions.py $(SIZE)

tar:
	mkdir thompsonH03
	cp thompsonH03.c vector.c vector.h fraction.c fraction.h Makefile thompsonH03/
	tar cfvz thompsonH03.tgz thompsonH03/
	rm -rf thompsonH03

clean:
	rm H03.out *.o thompsonH03.tgz fractions.txt

