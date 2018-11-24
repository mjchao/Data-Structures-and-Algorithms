#include "Deque.h"
#include "Profiling.h"
#include <deque>
#include <iostream>


using namespace dsalgo;


void ProfilePushPopBack(int deque_size, int num_runs) {
  std::vector<int> rand_elems = RandN(-100000, 100000, deque_size); 

  int64_t start = 0;
  int64_t stop = 0;
  
  Deque<int> test;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      test.PushBack(rand_elems[j]);
    }
    for (int j = 0; j < deque_size; ++j) {
      test.PopBack();
    }
  }
  stop = Clock::Now();

  std::cout << "dsalgo Deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");

  std::deque<int> test_std;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      test_std.push_back(rand_elems[j]);
    }
    for (int j = 0; j < deque_size; ++j) {
      test_std.pop_back();
    }
  }
  stop = Clock::Now();
  std::cout << "std::deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");
}


void ProfilePushPopBackVariousSizes() {
  std::cout << "=== Profiling Deque Push Back Small Size ===" << std::endl;
  ProfilePushPopBack(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Push Back Medium Size ===" << std::endl;
  ProfilePushPopBack(100, 10000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Push Back Large Size ===" << std::endl;
  ProfilePushPopBack(10000, 100);
  std::cout << "\n\n\n";
}


void ProfileAccess(int deque_size, int num_runs)
{
  std::vector<int> rand_elems = RandN(-100000, 100000, deque_size); 

  int64_t start = 0;
  int64_t stop = 0;

  Deque<int> test;
  for (int i = 0; i < deque_size; ++i) {
    test.PushBack(rand_elems[i]);
  }

  std::deque<int> test_std;
  for (int i = 0; i < deque_size; ++i) {
    test_std.push_back(rand_elems[i]);
  }

  int64_t sum = 0;

  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      sum += test[j];
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo Deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");

  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      sum += test_std[j];
    }
  }
  stop = Clock::Now();

  std::cout << "std::deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");

  // do something with sum so that it doesn't get optimized out
  if (sum == 12345) {
    std::cout << "DEBUG: " << sum << std::endl;
  }
}


void ProfileAccessVariousSizes() {
  std::cout << "=== Profiling Deque Access Small Size ===" << std::endl;
  ProfileAccess(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Access Medium Size ===" << std::endl;
  ProfileAccess(100, 10000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Access Large Size ===" << std::endl;
  ProfileAccess(10000, 100);
  std::cout << "\n\n\n";
}


void ProfilePushPopFront(int deque_size, int num_runs) {
  std::vector<int> rand_elems = RandN(-100000, 100000, deque_size); 

  int64_t start = 0;
  int64_t stop = 0;
  
  Deque<int> test;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      test.PushFront(rand_elems[j]);
    }
    for (int j = 0; j < deque_size; ++j) {
      test.PopFront();
    }
  }
  stop = Clock::Now();

  std::cout << "dsalgo Deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");

  std::deque<int> test_std;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < deque_size; ++j) {
      test_std.push_front(rand_elems[j]);
    }
    for (int j = 0; j < deque_size; ++j) {
      test_std.pop_front();
    }
  }
  stop = Clock::Now();
  std::cout << "std::deque" << std::endl;
  PrintStats(stop - start, num_runs * deque_size, "\t");
}


void ProfilePushPopFrontVariousSizes() {
  std::cout << "=== Profiling Deque Push/Pop Front Small Size ===" << std::endl;
  ProfilePushPopFront(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Push/Pop Front Medium Size ===" << std::endl;
  ProfilePushPopFront(100, 10000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Deque Push/Pop Front Large Size ===" << std::endl;
  ProfilePushPopFront(10000, 100);
  std::cout << "\n\n\n";
}


int main() {
  ReseedRand();
  ProfilePushPopBackVariousSizes();
  ProfileAccessVariousSizes();
  ProfilePushPopFrontVariousSizes();
  return 0;
}

