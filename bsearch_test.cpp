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


void AssertLastOccurrence(const std::vector<int>& elems, int val,
    int expected) {
  if (expected != static_cast<int>(elems.size())) {
    assert(LastOccurrence(elems.begin(), elems.end(), val) ==
        elems.begin() + expected);
  } else {
    assert(LastOccurrence(elems.begin(), elems.end(), val) == elems.end());
  }
}


void testLastOccurrence() {
  // test single elem
  std::vector<int> elems = {0};
  AssertLastOccurrence(elems, 0, 0);

  elems = {0, 1, 2, 3, 4, 5};
  for (int i = 0; i < static_cast<int>(elems.size()); ++i) {
    AssertLastOccurrence(elems, i, i);
  }

  elems = {0, 0, 0, 0, 0};
  AssertLastOccurrence(elems, 0, 4);

  elems = {0, 0, 1, 1, 1};
  AssertLastOccurrence(elems, 0, 1);
  AssertLastOccurrence(elems, 1, 4);

  elems = {1, 1, 3, 3, 3};
  AssertLastOccurrence(elems, 0, 5);
  AssertLastOccurrence(elems, 2, 5);
  AssertLastOccurrence(elems, 4, 5);
}


void testLastOccurrenceRandomized() {
  int num_elems = 10000;
  int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {
    std::vector<int> test = RandN(-100, 100, num_elems);
    std::sort(test.begin(), test.end());
    for (int i = 0; i < num_elems - 1; ++i) {
      if (test[i] != test[i+1]) {
        AssertLastOccurrence(test, test[i], i);
      }
    }
  }
}


void AssertUpperBound(const std::vector<int>& elems, int val, int expected) {
  if (expected != static_cast<int>(elems.size())) {
    assert(UpperBound(elems.begin(), elems.end(), val) ==
        elems.begin() + expected);
  } else {
    assert(UpperBound(elems.begin(), elems.end(), val) == elems.end());
  }
}


void testUpperBound() {
  // test single elem
  std::vector<int> elems = {1};
  AssertUpperBound(elems, 0, 0);

  // test two elems
  elems = {1, 2};
  AssertUpperBound(elems, 0, 0);
  AssertUpperBound(elems, 1, 1);

  // test three elems
  elems = {1, 2, 3};
  AssertUpperBound(elems, 0, 0);
  AssertUpperBound(elems, 1, 1);
  AssertUpperBound(elems, 2, 2);

  // test upper bound exists, but search value not in the array
  elems = {1};
  AssertUpperBound(elems, 0, 0);

  elems = {1, 3};
  AssertUpperBound(elems, 2, 1);

  elems = {1, 3, 5};
  AssertUpperBound(elems, 4, 2);

  // test upper not found
  elems = {};
  AssertUpperBound(elems, 0, 0);

  elems = {1, 2, 3};
  AssertUpperBound(elems, 3, 3);

  elems = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; ++i) {
    AssertUpperBound(elems, i, i);
  }
}


void testUpperBoundRandomized() {
  int num_elems = 10000;
  int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {
    std::vector<int> test = RandN(-100, 100, num_elems);
    std::sort(test.begin(), test.end());
    for (int i = 0; i < num_elems - 1; ++i) {
      if (test[i] != test[i+1]) {
        AssertUpperBound(test, test[i], i+1);
      }
    }
  }
}


int main() {
  ReseedRand();
  testLowerBound();
  testLowerBoundRandomized();
  testLastOccurrence();
  testLastOccurrenceRandomized();
  testUpperBound();
  testUpperBoundRandomized();
  return 0;
}

