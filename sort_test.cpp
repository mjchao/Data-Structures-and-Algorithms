#include "Sort.h"
#include "Random.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>


using namespace dsalgo;


void AssertQuicksort(std::vector<int> input,
    const std::vector<int> expected) {
  Quicksort(input.begin(), input.end());
  assert(input == expected);
}


void testQuicksort() {
  std::vector<int> test;
  std::vector<int> expected;

  // empty
  Quicksort(test.begin(), test.end());
  AssertQuicksort(test, expected);

  // one element
  //AssertQuicksort({1}, {1});

  // two elements in order
  AssertQuicksort({1, 2}, {1, 2});

  // two elements reverse order
  AssertQuicksort({2, 1}, {1, 2});

  // all orderings of three elements
  AssertQuicksort({1, 2, 3}, {1, 2, 3});
  AssertQuicksort({1, 3, 2}, {1, 2, 3});
  AssertQuicksort({2, 1, 3}, {1, 2, 3});
  AssertQuicksort({2, 3, 1}, {1, 2, 3});
  AssertQuicksort({3, 1, 2}, {1, 2, 3});
  AssertQuicksort({3, 2, 1}, {1, 2, 3});

  // all elements are the same
  AssertQuicksort({1, 1, 1, 1, 1}, {1, 1, 1, 1, 1});
  
  // alternating high and low
  AssertQuicksort({1, 9, 2, 8, 3, 7, 4, 6, 5, 5},
      {1, 2, 3, 4, 5, 5, 6, 7, 8, 9});
}

void testQuicksortRandomized() {
  int num_elems = 1000;
  int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {
    std::vector<int> dsalgo_sort = RandN(-100000, 100000, num_elems);
    std::vector<int> std_sort = dsalgo_sort;
    Quicksort(dsalgo_sort.begin(), dsalgo_sort.end());
    std::sort(std_sort.begin(), std_sort.end());
    assert(dsalgo_sort == std_sort);
  }
}


int main() {
  testQuicksort();
  testQuicksortRandomized();
  return 0;
}
