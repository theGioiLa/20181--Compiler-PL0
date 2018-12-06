main: main.cpp parser.cpp scanner.cpp symbol.cpp
	g++ -std=c++11 $^ -o main

clean:
	rm main
