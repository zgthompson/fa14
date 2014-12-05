CC = gcc
CFLAGS = -Wall

all: P1.out

P1.out: Project1.o SetLimits.o lex.o
	$(CC) $(CFLAGS) Project1.o SetLimits.o lex.o -o P1.out

Project1.o: Project1.c
	$(CC) $(CFLAGS) -c Project1.c

SetLimits.o: SetLimits.c
	$(CC) $(CFLAGS) -c SetLimits.c

lex.o: lex.c
	$(CC) $(CFLAGS) -c lex.c

tar:
	mkdir thompsonP1
	cp Makefile Project1.c lex.h lex.c state_table.h SetLimits.c SetLimits.h thompsonP1/
	tar cfvz thompsonP1.tgz thompsonP1/
	rm -rf thompsonP1

clean:
	rm P1.out *.o thompsonP1.tgz

