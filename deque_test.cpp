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

void testPushFront() {
  // test pushing front into a deque with the head at index 0. The head needs
  // to wrap around back to the end of the underlying array.
  Deque<int> test;
  test.PushBack(1);
  test.PushFront(0);
  assert(test.Front() == 0);
  assert(test.Back() == 1);
  test.PushFront(-1);
  assert(test.Front() == -1);
  assert(test[1] == 0);
  assert(test.Back() == 1);

  test.Clear();

  // now test just pushing front a bunch of elements.
  int num_elems = 1028;
  for (int i = 0; i < num_elems; ++i) {
    test.PushFront(i);
    for (int j = 0; j <= i; ++j) {
      assert(test[j] == (i - j));
    }
  }
}


int main() {
  testPushBack();
  testPushBackWithPopFront();
  testPushFront();
}

