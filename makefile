CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror
OPT=-O3
DEBUG=-g

vector:
	$(CXX) $(CXXFLAGS) $(OPT) vector_prof.cpp -o vector_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) vector_test.cpp -o vector_test-dbg

