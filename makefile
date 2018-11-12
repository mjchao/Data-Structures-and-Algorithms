CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror
OPT=-O3 -DNDEBUG
DEBUG=-g

vector:
	$(CXX) $(CXXFLAGS) $(OPT) vector_prof.cpp -o vector_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) vector_test.cpp -o vector_test-dbg


lru:
	$(CXX) $(CXXFLAGS) $(OPT) lru_prof.cpp -o lru_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) lru_test.cpp -o lru_test-dbg

clean:
	rm *prof-opt *test-dbg

