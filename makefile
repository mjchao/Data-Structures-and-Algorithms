CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror
OPT=-O3 -DNDEBUG
DEBUG=-g

all: vector lru deque bsearch sort hashmap

vector:
	$(CXX) $(CXXFLAGS) $(OPT) vector_prof.cpp -o vector_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) vector_test.cpp -o vector_test-dbg

lru:
	$(CXX) $(CXXFLAGS) $(OPT) lru_prof.cpp -o lru_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) lru_test.cpp -o lru_test-dbg

deque:
	$(CXX) $(CXXFLAGS) $(OPT) deque_prof.cpp -o deque_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) deque_test.cpp -o deque_test-dbg

bsearch:
	$(CXX) $(CXXFLAGS) $(OPT) bsearch_prof.cpp -o bsearch_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) bsearch_test.cpp -o bsearch_test-dbg

# TODO profile optimized sort
sort:
	$(CXX) $(CXXFLAGS) $(DEBUG) sort_test.cpp -o sort_test-dbg

hashmap:
	$(CXX) $(CXXFLAGS) $(OPT) hashmap_prof.cpp -o hashmap_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) hashmap_test.cpp -o hashmap_test-dbg

triemap:
	$(CXX) $(CXXFLAGS) $(OPT) triemap_prof.cpp -o triemap_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) triemap_test.cpp -o triemap_test-dbg

shmqueue:
	$(CXX) $(CXXFLAGS) $(OPT) shmqueue_prof.cpp -o shmqueue_prof-opt
	$(CXX) $(CXXFLAGS) $(DEBUG) shmqueue_test.cpp -o shmqueue_test-dbg

clean:
	rm *prof-opt *test-dbg

