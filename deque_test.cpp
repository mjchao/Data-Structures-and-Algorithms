#include "Deque.h"
#include <assert.h>
#include <iostream>


using namespace dsalgo;


void testPushBack() {
  Deque<int> test;
  for (int i = 0; i < 1028; ++i) {
    test.PushBack(i);
    for (int j = 0; j <= i; ++j) {
      assert(test[j] == j);
    }
  }

  // destructor implicitly tested here
}


void testPushBackWithPopFront() {
  Deque<int> test;
  int num_elems = 1028;
  for (int i = 0; i < num_elems; ++i) {
    test.PushBack(i);

    // pop the first half of the numbers
    if (i % 2 == 0) {
      test.PopFront();
    }
  }
  for (int i = 0; i < num_elems / 2; ++i) {
    assert(test[i] == (num_elems / 2) + i);
  }
}


int main() {
  testPushBack();
}

