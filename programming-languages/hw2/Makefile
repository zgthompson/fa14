all: H02.out


H02.out: thompsonH02.o fraction.o
	g++ thompsonH02.o fraction.o -o H02.out

thompsonH02.o: thompsonH02.cpp
	g++ -c thompsonH02.cpp

fraction.o: fraction.cpp
	g++ -c fraction.cpp

clean:
	rm H02.out *.o
