all:
	g++ thompsonH02.cpp fraction.cpp -o HW02.out

HW02.out: thompsonH02.o fraction.o
	g++ thompsonH02.o fraction.o -o H02.out

thompsonH02.o: thompsonH02.cpp
	g++ -c thompsonH02.cpp

fraction.o: fraction.cpp
	g++ -c fraction.cpp

clean:
	rm HW02.out
