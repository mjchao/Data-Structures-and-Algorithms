#include "Vector.h"
#include "Profiling.h"
#include <vector>
#include <iostream>


using namespace dsalgo;


void profileConstructorLarge() {
	std::cout << "=== Profiling Vector Constructor Large Size ===" << std::endl;
  const int N = 10000;
	int min_size = 10000;
	int max_size = 10005;
	int64_t start = 0, stop = 0;
	std::vector<int> rand_sizes;

  rand_sizes = randn(min_size, max_size, N);
  start = Clock::now(); 
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    Vector<int> test(rand_sizes[i]);
  }
  stop = Clock::now();
  std::cout << "dsalgo Vector constructor time: " << stop - start << std::endl;

  rand_sizes = randn(min_size, max_size, N);
  start = Clock::now();
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    std::vector<int> test(rand_sizes[i]);
  }
  stop = Clock::now();
  std::cout << "std::vector constructor time: " << stop - start << std::endl;

}


int main() {
  profileConstructorLarge();
  return 0;
}
