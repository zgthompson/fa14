all: H02.out


H02.out: thompsonH02.o fraction.o
	g++ thompsonH02.o fraction.o -o H02.out

thompsonH02.o: thompsonH02.cpp
	g++ -c thompsonH02.cpp

fraction.o: fraction.cpp
	g++ -c fraction.cpp

tar:
	mkdir thompsonH02
	cp thompsonH02.cpp fraction.cpp fraction.h Makefile thompsonH02/
	tar cfvz thompsonH02.tgz thompsonH02/
	rm -rf thompsonH02

clean:
	rm H02.out *.o thompsonH02.tgz
