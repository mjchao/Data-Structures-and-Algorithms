#include "BinarySearch.h"
#include "Profiling.h"
#include <algorithm>
#include <vector>


using namespace dsalgo;


void ProfileBinarySearch(int range_size, int num_runs) {
  std::vector<int> rand_elems = RandN(0, range_size, range_size);
  std::sort(rand_elems.begin(), rand_elems.end());

  int64_t start = 0;
  int64_t stop = 0;

  // profile lower bound
  start = Clock::Now();
  for (int i = 0 ; i < num_runs; ++i) {
    for (int j = 0; j < range_size; ++j) {
      LowerBound(rand_elems.begin(), rand_elems.end(), i);
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo LowerBound" << std::endl;
  PrintStats(stop - start, range_size * num_runs, "\t");

  // profile last occurrence
  start = Clock::Now();
  for (int i = 0 ; i < num_runs; ++i) {
    for (int j = 0; j < range_size; ++j) {
      LastOccurrence(rand_elems.begin(), rand_elems.end(), i);
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo LastOccurrence" << std::endl;
  PrintStats(stop - start, range_size * num_runs, "\t");

  // profile upper bound
  start = Clock::Now();
  for (int i = 0 ; i < num_runs; ++i) {
    for (int j = 0; j < range_size; ++j) {
      UpperBound(rand_elems.begin(), rand_elems.end(), i);
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo UpperBound" << std::endl;
  PrintStats(stop - start, range_size * num_runs, "\t");

  // profile std::lower_bound
  start = Clock::Now();
  for (int i = 0 ; i < num_runs; ++i) {
    for (int j = 0; j < range_size; ++j) {
      std::lower_bound(rand_elems.begin(), rand_elems.end(), i);
    }
  }
  stop = Clock::Now();
  std::cout << "std::lower_bound" << std::endl;
  PrintStats(stop - start, range_size * num_runs, "\t");

  // profile std::upper_bound
  start = Clock::Now();
  for (int i = 0 ; i < num_runs; ++i) {
    for (int j = 0; j < range_size; ++j) {
      std::upper_bound(rand_elems.begin(), rand_elems.end(), i);
    }
  }
  stop = Clock::Now();
  std::cout << "std::upper_bound" << std::endl;
  PrintStats(stop - start, range_size * num_runs, "\t");
}


int main() {
  std::cout << "=== Profiling Binary Search Small Range ===" << std::endl;
  ProfileBinarySearch(10, 10000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Binary Search Medium Range ===" << std::endl;
  ProfileBinarySearch(100, 1000);
  std::cout << "\n\n\n";


  std::cout << "=== Profiling Binary Search Large Range ===" << std::endl;
  ProfileBinarySearch(10000, 10);
  std::cout << "\n\n\n";
  return 0;
}

