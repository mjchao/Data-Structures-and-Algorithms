#include "Deque.h"
#include "Profiling.h"
#include <assert.h>
#include <iostream>
#include <deque>


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


void testRandomized() {
  std::deque<int> correct_deque;
  Deque<int> test_deque;

  int num_ops = 20;
  for (int i = 0; i < num_ops; ++i) {
    int operation = RandInt(0, 3);

    // push back
    if (operation == 0) {
      int rand_val = RandInt(-10000, 10000);
      correct_deque.push_back(rand_val);
      test_deque.PushBack(rand_val);

    // push front
    } else if (operation == 1) {
      int rand_val = RandInt(-10000, 10000);
      correct_deque.push_front(rand_val);
      test_deque.PushFront(rand_val);

    // pop back
    } else if (operation == 2 && correct_deque.size() > 0) {
      correct_deque.pop_back();
      test_deque.PopBack();

    // pop front
    } else if (operation == 3 && correct_deque.size() > 0) {
      correct_deque.pop_front();
      test_deque.PopFront();
    }
    assert(static_cast<int>(correct_deque.size()) == test_deque.Size());
    for (int i = 0; i < test_deque.Size(); ++i) {
      assert(correct_deque[i] == test_deque[i]);
    }
  }
}


int main() {
  testPushBack();
  testPushBackWithPopFront();
  testPushFront();
  testRandomized();
}

