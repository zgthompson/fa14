CC = gcc
CFLAGS = -Wall
SIZE = 1000

all:
	$(CC) $(CFLAGS) fractions.c -o fractions.o

fractions:
	python generateFractions.py $(SIZE)

tar:
	tar cvzf thompsonH03.tar.gz fractions.c Makefile

clean:
	rm fractions.o fractions.txt

