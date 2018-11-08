#include "Vector.h"
#include "Profiling.h"
#include <vector>
#include <iostream>


using namespace dsalgo;


void ProfileConstructor(int min_size, int max_size, int N) {
  std::vector<int> rand_sizes;
  int64_t start = 0;
  int64_t stop = 0;

  rand_sizes = RandN(min_size, max_size, N);
  start = Clock::Now(); 
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    Vector<int> test(rand_sizes[i]);
  }
  stop = Clock::Now();
  std::cout << "dsalgo Vector" << std::endl;
  std::cout << "\tTotal Time: " << stop - start << std::endl;
  std::cout << "\tTotal Calls:" << N << std::endl;
  std::cout << "\tns/call: " << ComputeTimePerCall(stop - start, N)
    << std::endl;
  
  //rand_sizes = RandN(min_size, max_size, N);
  start = Clock::Now();
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    std::vector<int> test(rand_sizes[i]);
  }
  stop = Clock::Now();
  std::cout << "std::vector" << std::endl;
  std::cout << "\tTotal Time: " << stop - start << std::endl;
  std::cout << "\tTotal Calls:" << N << std::endl;
  std::cout << "\tns/call: " << ComputeTimePerCall(stop - start, N)
    << std::endl;
}

void ProfileConstructorLarge() {
	std::cout << "=== Profiling Vector Constructor Large Size ===" << std::endl;
  const int N = 10000;
	int min_size = 10000;
	int max_size = 10005;
  ProfileConstructor(min_size, max_size, N);
  std::cout << "\n\n\n";
}

void ProfileConstructorSmall() {
  std::cout << "=== Profiling Vector Constructor Small size ===" << std::endl;
  const int N = 10000;
  int min_size = 10;
  int max_size = 15;
  ProfileConstructor(min_size, max_size, N);
  std::cout << "\n\n\n";
}

int main() {
  ProfileConstructorLarge();
  ProfileConstructorSmall();
  return 0;
}
