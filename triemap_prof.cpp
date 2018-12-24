#include "Triemap.h"
#include "Hashmap.h"
#include "Profiling.h"
#include "Random.h"
#include <iostream>


using namespace dsalgo;


// TODO see if possible to optimize trie better. The linear search through
// a linked list is slow
void ProfilePut(int num_inserts, int num_runs) {
  std::vector<std::string> rand_elems = RandStrs(8, 16, num_inserts);

  int64_t total_time = 0;
  int64_t start = 0;
  int64_t stop = 0;

  Triemap<std::string, std::string> test;
  for (int i = 0; i < num_runs; ++i) {
    start = Clock::Now();
    for (const std::string& e : rand_elems) {
      test.Put(e, e);
    }
    stop = Clock::Now();
    total_time += (stop - start);
    test.Clear();
  }
  std::cout << "dsalgo Triemap" << std::endl;
  PrintStats(total_time, num_runs * num_inserts, "\t");

  Hashmap<std::string, std::string> test_hashmap;
  total_time = 0;
  for (int i = 0; i < num_runs; ++i) {
    start = Clock::Now();
    for (const std::string& e : rand_elems) {
      test_hashmap.Put(e, e); 
    }
    stop = Clock::Now();
    total_time += (stop - start);
    test.Clear();
  }
  std::cout << "dsalgo Hashmap" << std::endl;
  PrintStats(total_time, num_runs * num_inserts, "\t");
}


void ProfilePutVariousSizes() {
  std::cout << "=== Profiling Triemap Put Small Size ===" << std::endl;
  ProfilePut(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Triemap Put Medium Size ===" << std::endl;
  ProfilePut(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Triemap Put Large Size ===" << std::endl;
  ProfilePut(100000, 10);
  std::cout << "\n\n\n";
}


int main() {
  ProfilePutVariousSizes();
  return 0;
}

