#include "LruQueue.h"
#include "Profiling.h"
#include "Random.h"
#include <iostream>
#include <vector>


using namespace dsalgo;


void ProfileConstructionInt64(int num_elems, int num_runs) {
  std::vector<int64_t> elems;
  for (int i = 0; i < num_elems; ++i) {
    elems.push_back(i);
  }

  int64_t start = 0;
  int64_t stop = 0;

  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    LruQueue<int64_t> test_queue(elems);
  }
  stop = Clock::Now();
  std::cout << "dsalgo lru" << std::endl;
  PrintStats(stop - start, num_elems * num_runs, "\t");
}


void ProfileConstruction() {

  // note: If you wanted to use an LruQueue for a custom object, it'd be
  // faster and more memory-efficient to use pointers to those objects.
  // That speed would be practically equivalent to Lru<int64_t> because
  // both pointers and int64_t are 8 bytes. Maybe you get a small <1ns slowdown
  // from accessing a pointer, but that should be nearly negligible.
  std::cout << "=== Profile small Lru construction ===" << std::endl;
  ProfileConstructionInt64(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile medium Lru construction ===" << std::endl;
  ProfileConstructionInt64(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile large Lru construction ===" << std::endl;
  ProfileConstructionInt64(1000000, 1);
  std::cout << "\n\n\n";
}


void ProfileGetAndUseFront(int num_elems, int num_runs) {
  std::vector<int64_t> elems;
  for (int i = 0; i < num_elems; ++i) {
    elems.push_back(i);
  }

  LruQueue<int64_t> test(elems);
  int64_t start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    test.MarkUsed(test.GetLru());
  }
  int64_t stop = Clock::Now();
  std::cout << "dsalgo lru" << std::endl;
  PrintStats(stop - start, num_elems * num_runs, "\t");
}


void ProfileGetAndUseBack(int num_elems, int num_runs) {
  // doesn't work if number of elements less than 3
  assert(num_elems > 3);
  std::vector<int64_t> elems;
  std::vector<int64_t> elem_to_mark_used;
  for (int i = 0; i < num_elems; ++i) {
    elems.push_back(i);

    // Precompute this outside of profiling loop because this alternating
    // branching could have significant time penalty.
    if (i % 2 == 0) {
      elem_to_mark_used.push_back(num_elems - 2);
    } else {
      elem_to_mark_used.push_back(num_elems - 1);
    }
  }

  LruQueue<int64_t> test(elems);
  int64_t start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < num_elems; ++j) {
      test.MarkUsed(elem_to_mark_used[j]);
    }
  }
  int64_t stop = Clock::Now();
  std::cout << "dsalgo lru" << std::endl;
  PrintStats(stop - start, num_elems * num_runs, "\t");
}


void ProfileGetAndUseRandom(int num_elems, int num_runs) {
  std::vector<int64_t> elems;
  for (int i = 0; i < num_elems; ++i) {
    elems.push_back(i);
  }
  std::vector<int> elem_to_mark_used = RandN(0, num_elems - 1, num_runs);

  LruQueue<int64_t> test(elems);
  int64_t start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    test.MarkUsed(elem_to_mark_used[i]);
  }
  int64_t stop = Clock::Now();
  std::cout << "dsalgo lru" << std::endl;
  PrintStats(stop - start, num_runs, "\t");
}


void ProfileGetAndUse() {
  std::cout << "=== Profile small Lru get + use front ===" << std::endl;
  ProfileGetAndUseFront(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile medium Lru get + use front ===" << std::endl;
  ProfileGetAndUseFront(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile large Lru get + use front ===" << std::endl;
  ProfileGetAndUseFront(1000000, 1);
  std::cout << "\n\n\n";

  std::cout << "=== Profile small Lru use back ===" << std::endl;
  ProfileGetAndUseBack(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile medium Lru use back ===" << std::endl;
  ProfileGetAndUseBack(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile medium Lru use back ===" << std::endl;
  ProfileGetAndUseBack(1000000, 1);
  std::cout << "\n\n\n";

  std::cout << "=== Profile small lru use random ===" << std::endl;
  ProfileGetAndUseRandom(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile medium lru use random ===" << std::endl;
  ProfileGetAndUseRandom(1000, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profile large lru use random ===" << std::endl;
  ProfileGetAndUseRandom(1000000, 100000);
  std::cout << "\n\n\n";
}


int main() {
  ReseedRand();
  ProfileConstruction();
  ProfileGetAndUse();
  return 0;
}

