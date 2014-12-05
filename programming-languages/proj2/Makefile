CC = gcc
CFLAGS = -Wall

all: P2.out

P2.out: Project2.o SetLimits.o lex.o syn.o
	$(CC) $(CFLAGS) Project2.o SetLimits.o lex.o syn.o -o P2.out

Project2.o: Project2.c
	$(CC) $(CFLAGS) -c Project2.c

SetLimits.o: SetLimits.c
	$(CC) $(CFLAGS) -c SetLimits.c

lex.o: lex.c
	$(CC) $(CFLAGS) -c lex.c

syn.o: syn.c
	$(CC) $(CFLAGS) -c syn.c

tar:
	mkdir thompsonP2
	cp Makefile Project2.c lex.h lex.c syn.h syn.c state_table.h SetLimits.c SetLimits.h thompsonP2/
	tar cfvz thompsonP2.tgz thompsonP2/
	rm -rf thompsonP2

clean:
	rm P2.out *.o thompsonP2.tgz

