CXX=g++
CXXFLAGS=-c -O2

all: main.o Rubik.o Constants.o Algorithm.o PrettyPrint.o
	$(CXX) main.o Rubik.o Constants.o Algorithm.o PrettyPrint.o -o r

main.o: main.cpp Rubik.h Constants.h Algorithm.h PrettyPrint.h
	$(CXX) $(CXXFLAGS) main.cpp

Algorithm.o: Algorithm.cpp Algorithm.h Rubik.h
	$(CXX) $(CXXFLAGS) Algorithm.cpp

Rubik.o: Rubik.cpp Rubik.h Constants.h
	$(CXX) $(CXXFLAGS) Rubik.cpp

Constants.o: Constants.cpp Constants.h
	$(CXX) $(CXXFLAGS) Constants.cpp

PrettyPrint.o: PrettyPrint.cpp PrettyPrint.h
	$(CXX) $(CXXFLAGS) PrettyPrint.cpp

clean:
	rm -f *.o r
