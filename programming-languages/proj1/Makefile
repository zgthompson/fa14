CC = gcc
CFLAGS = -Wall

all: P1.out

P1.out: proj1.o
	$(CC) $(CFLAGS) proj1.o -o P1.out

proj1.o: proj1.c
	$(CC) $(CFLAGS) -c proj1.c

tar:
	mkdir thompsonP1
	cp Makefile proj1.c lex.h state_table.h thompsonP1/
	tar cfvz thompsonP1.tgz thompsonP1/
	rm -rf thompsonP1

clean:
	rm P1.out *.o thompsonP1.tgz

