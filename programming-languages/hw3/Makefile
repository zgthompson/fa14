CC = gcc
CFLAGS = -Wall
SIZE = 1000

all: H03.out

H03.out: thompsonH03.o vector.o fraction.o quicksort.o SetLimits.o
	$(CC) $(CFLAGS) thompsonH03.o vector.o fraction.o quicksort.o SetLimits.o -o H03.out

thompsonH03.o: thompsonH03.c
	$(CC) $(CFLAGS) -c thompsonH03.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

fraction.o: fraction.c fraction.h
	$(CC) $(CFLAGS) -c fraction.c

quicksort.o: quicksort.c quicksort.h
	$(CC) $(CFLAGS) -c quicksort.c

SetLimits.o: SetLimits.c SetLimits.h
	$(CC) $(CFLAGS) -c SetLimits.c

run:
	time ./H03.out fractions.txt > test

fractions:
	python generateFractions.py $(SIZE)

tar:
	mkdir thompsonH03
	cp thompsonH03.c SetLimits.h SetLimits.c vector.c vector.h fraction.c fraction.h quicksort.c quicksort.h Makefile thompsonH03/
	tar cfvz thompsonH03.tgz thompsonH03/
	rm -rf thompsonH03

clean:
	rm H03.out *.o thompsonH03.tgz fractions.txt

