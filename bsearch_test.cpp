#include "BinarySearch.h"
#include "Random.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>


using namespace dsalgo;


void AssertLowerBound(const std::vector<int>& elems, int val, int expected) {
  if (expected != static_cast<int>(elems.size())) {
    assert(LowerBound(elems.begin(), elems.end(), val) ==
        elems.begin() + expected);
  } else {
    assert(LowerBound(elems.begin(), elems.end(), val) == elems.end());
  }
}


void testLowerBound() {
  // test single elem
  std::vector<int> elems = {0};
  AssertLowerBound(elems, 0, 0);

  // test 2 elems
  elems = {0, 1};
  AssertLowerBound(elems, 0, 0);
  AssertLowerBound(elems, 1, 1);

  // test 3 elems
  elems = {0, 1, 2};
  AssertLowerBound(elems, 0, 0);
  AssertLowerBound(elems, 1, 1);
  AssertLowerBound(elems, 2, 2);

  // test no lower bound found
  elems = {};
  AssertLowerBound(elems, 0, 0);

  elems = {0};
  AssertLowerBound(elems, 1, 1);

  elems = {0, 1};
  AssertLowerBound(elems, 2, 2);

  elems = {0, 1, 2};
  AssertLowerBound(elems, 3, 3);

  // test lower bound is different from search value
  elems = {1};
  AssertLowerBound(elems, 0, 0);

  elems = {1, 3};
  AssertLowerBound(elems, 2, 1);

  elems = {1, 3, 5};
  AssertLowerBound(elems, 4, 2);

  // test multiple of valid values for lower bound
  elems = {1, 1, 1};
  AssertLowerBound(elems, 1, 0);

  elems = {0, 1, 1, 1, 1};
  AssertLowerBound(elems, 1, 1);

  elems = {0, 2, 2, 2, 2};
  AssertLowerBound(elems, 1, 1);

  elems = {2, 2, 2};
  AssertLowerBound(elems, 1, 0);
}


void testLowerBoundRandomized() {
  int num_elems = 10000;
  int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {
    std::vector<int> test = RandN(-100, 100, num_elems);
    std::sort(test.begin(), test.end());
    for (int i = 0; i < num_elems - 1; ++i) {
      if (test[i] != test[i+1]) {
        AssertLowerBound(test, test[i+1], i+1);
      }
    }
  }
}


void AssertLowerBoundLast(const std::vector<int>& elems, int val,
    int expected) {
  if (expected != static_cast<int>(elems.size())) {
    assert(LowerBoundLast(elems.begin(), elems.end(), val) ==
        elems.begin() + expected);
  } else {
    assert(LowerBoundLast(elems.begin(), elems.end(), val) == elems.end());
  }
}


void testLowerBoundLast() {
  // test single elem
  std::vector<int> elems = {0};
  AssertLowerBoundLast(elems, 0, 0);

  elems = {0, 1, 2, 3, 4, 5};
  for (int i = 0; i < static_cast<int>(elems.size()); ++i) {
    AssertLowerBoundLast(elems, i, i);
  }

  elems = {0, 0, 0, 0, 0};
  AssertLowerBoundLast(elems, 0, 4);

  elems = {0, 0, 1, 1, 1};
  AssertLowerBoundLast(elems, 0, 1);
  AssertLowerBoundLast(elems, 1, 4);

  elems = {1, 1, 3, 3, 3};
  AssertLowerBoundLast(elems, 2, 2);
  AssertLowerBoundLast(elems, 4, 5);
}


void testLowerBoundLastRandomized() {
  int num_elems = 10000;
  int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {
    std::vector<int> test = RandN(-100, 100, num_elems);
    std::sort(test.begin(), test.end());
    for (int i = 0; i < num_elems - 1; ++i) {
      if (test[i] != test[i+1]) {
        AssertLowerBoundLast(test, test[i], i);
      }
    }
  }
}


int main() {
  ReseedRand();
  testLowerBound();
  testLowerBoundRandomized();
  testLowerBoundLast();
  testLowerBoundLastRandomized();
  return 0;
}

