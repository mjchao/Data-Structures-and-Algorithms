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


void testErase() {
  Deque<int> test;

  // === test only having to shift the non-wrapped-around part, with the head
  //     being at index 0 === //
  for (int i = 0; i <= 28; ++i) {
    test.PushBack(i);
  }

  // erase all the even numbers
  for (int i = 0; i <= test.Size(); ++i) {
    test.Erase(i);
  }
  for (int i = 0; i < test.Size(); ++i) {
    assert(test[i] == 2*i + 1);
  }

  test.Clear();


  // === test only having to shift the non-wrapped-around part, with the head
  //     being at a non-zero index === //
  
  // shift the head index a bit
  for (int i = 0; i < 3; ++i) {
    test.PushBack(0);
    test.PopFront();
  }

  // test erasing
  for (int i = 0; i < 3; ++i) {
    test.PushBack(i);
  }
  test.Erase(1);
  assert(test.Size() == 2);
  assert(test[0] == 0);
  assert(test[1] == 2);

  test.Clear();

  // === test shifting only the wrapped-around part === //

  // shift the head index a bit
  for (int i = 0; i < 3; ++i) {
    test.PushBack(0);
    test.PopFront();
  }

  for (int i = 0; i < 8; ++i) {
    test.PushBack(i); 
  }

  test.Erase(7);
  for (int i = 0; i < test.Size(); ++i) {
    assert(test[i] == i);
  }
  test.PushBack(7);

  test.Erase(6);
  for (int i = 0; i < test.Size(); ++i) {
    if (i < 6) {
      assert(test[i] == i);
    } else {
      assert(test[i] == i + 1);
    }
  }

  test.Erase(5);
  for (int i = 0; i < 5; ++i) {
    assert(test[i] == i); 
  }
  assert(test[5] == 7);
  
  test.Clear();

  // === test shifting both wrapped and non-wrapped parts === //

  // shift the head index a bit
  for (int i = 0; i < 3; ++i) {
    test.PushBack(0);
    test.PopFront();
  }

  for (int i = 0; i < 8; ++i) {
    test.PushBack(i);
  }
  for (int i = 4; i >= 0; --i) {
    test.Erase(i);
    for (int j = 0; j < i; ++j) {
      assert(test[j] == j);
    }
    assert(test[i] == 5);
    assert(test[i+1] == 6);
    assert(test[i+2] == 7);
  }
}


void testRandomized() {
  std::deque<int> correct_deque;
  Deque<int> test_deque;

  int num_ops = 2000;
  for (int i = 0; i < num_ops; ++i) {
    int operation = RandInt(0, 100);

    // push back
    if (0 <= operation && operation < 25) {
      int rand_val = RandInt(-10000, 10000);
      correct_deque.push_back(rand_val);
      test_deque.PushBack(rand_val);

    // push front
    } else if (25 <= operation && operation < 50) {
      int rand_val = RandInt(-10000, 10000);
      correct_deque.push_front(rand_val);
      test_deque.PushFront(rand_val);

    // TODO insert
    } else if (50 <= operation && operation < 75) {

    // pop back
    } else if (75 <= operation && operation < 80 && correct_deque.size() > 0) {
      correct_deque.pop_back();
      test_deque.PopBack();

    // pop front
    } else if (80 <= operation && operation < 90 && correct_deque.size() > 0) {
      correct_deque.pop_front();
      test_deque.PopFront();

    // erase
    } else if (90 <= operation && operation < 100 && correct_deque.size() > 0) {
      int erase_idx = RandInt(0, correct_deque.size() - 1);
      correct_deque.erase(correct_deque.begin() + erase_idx);
      test_deque.Erase(erase_idx);
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
  testErase();
  testRandomized();
}

