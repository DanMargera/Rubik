CXX=g++
CXXFLAGS=-c -O2

all: main.o Rubik.o Constants.o Algorithm.o
	$(CXX) main.o Rubik.o Constants.o Algorithm.o -o r

main.o: windows_main.cpp Rubik.h Constants.h Algorithm.h
	$(CXX) $(CXXFLAGS) windows_main.cpp

Algorithm.o: Algorithm.cpp Algorithm.h Rubik.h
	$(CXX) $(CXXFLAGS) Algorithm.cpp

Rubik.o: Rubik.cpp Rubik.h Constants.h
	$(CXX) $(CXXFLAGS) Rubik.cpp

Constants.o: Constants.cpp Constants.h
	$(CXX) $(CXXFLAGS) Constants.cpp

clean:
	rm -f *.o r
