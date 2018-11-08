#include "Vector.h"
#include "Profiling.h"
#include <vector>
#include <iostream>


using namespace dsalgo;


void ProfilePushBack(int min_size, int max_size, int num_runs) {
  std::vector<int> rand_sizes;
  std::vector<std::vector<int>> rand_vals;
  int64_t start = 0;
  int64_t stop = 0;

  // generate data to insert into vector
  int num_push_backs = 0;
  rand_sizes = RandN(min_size, max_size, num_runs);
  for (int i = 0 ; i < static_cast<int>(rand_sizes.size()); ++i) {
    num_push_backs += rand_sizes[i];
    rand_vals.push_back(RandN(-100000, 100000, rand_sizes[i]));
  }

  start = Clock::Now();
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    Vector<int> test;
    for (int j = 0; j < rand_sizes[i]; ++j) {
      test.PushBack(rand_vals[i][j]);
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo Vector" << std::endl;
  PrintStats(stop - start, num_push_backs, "\t");

  start = Clock::Now();
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    std::vector<int> test;
    for (int j = 0; j < rand_sizes[i]; ++j) {
      test.push_back(rand_vals[i][j]);
    }
  }
  stop = Clock::Now();
  std::cout << "std::vector" << std::endl;
  PrintStats(stop - start, num_push_backs, "\t");
}

void ProfilePushBackSmall() {
 	std::cout << "=== Profiling Vector Push Back Small ===" << std::endl;
  const int num_runs = 100000;
	int min_size = 1;
	int max_size = 10;
  ProfilePushBack(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}

void ProfilePushBackMed() {
 	std::cout << "=== Profiling Vector Push Back Small ===" << std::endl;
  const int num_runs = 1000;
	int min_size = 100;
	int max_size = 150;
  ProfilePushBack(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}


void ProfilePushBackLarge() {
 	std::cout << "=== Profiling Vector Push Back Large ===" << std::endl;
  const int num_runs = 10;
	int min_size = 100000;
	int max_size = 100005;
  ProfilePushBack(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}


void ProfileConstructor(int min_size, int max_size, int num_runs) {
  std::vector<int> rand_sizes;
  int64_t start = 0;
  int64_t stop = 0;
  rand_sizes = RandN(min_size, max_size, num_runs);

  start = Clock::Now(); 
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    Vector<int> test(rand_sizes[i]);
  }
  stop = Clock::Now();
  std::cout << "dsalgo Vector" << std::endl;
  PrintStats(stop - start, num_runs, "\t");
  
  start = Clock::Now();
  for (int i = 0; i < static_cast<int>(rand_sizes.size()); ++i) {
    std::vector<int> test(rand_sizes[i]);
  }
  stop = Clock::Now();
  std::cout << "std::vector" << std::endl;
  PrintStats(stop - start, num_runs, "\t");
}


void ProfileConstructorLarge() {
	std::cout << "=== Profiling Vector Constructor Large Size ===" << std::endl;
  const int num_runs = 100;
	int min_size = 100000;
	int max_size = 100005;
  ProfileConstructor(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}

void ProfileConstructorMed() {
	std::cout << "=== Profiling Vector Constructor Medium Size ===" << std::endl;
  const int num_runs = 1000;
	int min_size = 100;
	int max_size = 150;
  ProfileConstructor(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}


void ProfileConstructorSmall() {
  std::cout << "=== Profiling Vector Constructor Small size ===" << std::endl;
  const int num_runs = 100000;
  int min_size = 1;
  int max_size = 10;
  ProfileConstructor(min_size, max_size, num_runs);
  std::cout << "\n\n\n";
}

int main() {
  ProfileConstructorSmall();
  ProfileConstructorMed();
  ProfileConstructorLarge();

  ProfilePushBackSmall();
  ProfilePushBackMed();
  ProfilePushBackLarge();
  return 0;
}
