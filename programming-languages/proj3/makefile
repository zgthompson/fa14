P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o Interpreter.o
	g++ -g -o P3.out Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o Interpreter.o

Project3.o : Project3.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project3.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h Interpreter.h
	g++ -g -c SyntacticalAnalyzer.cpp

Interpreter.o : Interpreter.h Interpreter.cpp Symbol.h
	g++ -g -c Interpreter.cpp

tar:
	mkdir thompsonP3
	cp makefile README.txt *.cpp *.h thompsonP3/
	tar cfvz thompsonP3.tgz thompsonP3/
	rm -rf thompsonP3

clean : 
	rm *.o P3.out
