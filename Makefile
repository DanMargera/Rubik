CXX=g++
CXXFLAGS=-c -O2

all: main.o Rubik.o Constants.o
	$(CXX) main.o Rubik.o Constants.o -o r

main.o: main.cpp Rubik.h Constants.h
	$(CXX) $(CXXFLAGS) main.cpp

Rubik.o: Rubik.cpp Rubik.h Constants.h
	$(CXX) $(CXXFLAGS) Rubik.cpp

Constants.o: Constants.cpp Constants.h
	$(CXX) $(CXXFLAGS) Constants.cpp

clean:
	rm -f *.o r
