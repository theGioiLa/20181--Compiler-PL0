parser: main.cpp parser.cpp scanner.cpp symbol.cpp
	g++ -std=c++11 $^ -o parser 

scanner: test.cc scanner.cpp 
	g++ -std=c++11 $^ -o scanner

clean:
	rm parser scanner
