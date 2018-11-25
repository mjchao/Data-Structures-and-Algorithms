#include "Deque.h"
#include "Random.h"
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


template <typename T>
void PrintDeque(const Deque<T>& d) {
  for (int i = 0; i < d.Size(); ++i) {
    std::cout << d[i] << " ";
  }
  std::cout << std::endl;
}


void testInsert() {
  Deque<int> test;

  // === test inserting at the back === //
  int num_elems = 109;
  for (int i = 0; i < num_elems; ++i) {
    test.Insert(i, i);
    for (int j = 0; j <= i; ++j) {
      assert(test[j] == j);
    }
  }

  test.Clear();

  // === test inserting at the front === //
  for (int i = 0; i < num_elems; ++i) {
    test.Insert(i, 0);
    for (int j = 0; j <= i; ++j) {
      assert(test[j] == i - j);
    }
  }

  test.Clear();

  // === test inserting at the middle causing resize === //
  for (int i = 0; i < 8; ++i) {
    test.PushBack(i);
  }
  for (int i = 0; i < 10; ++i) {
    test.Insert(-1, 5);
  }

  // array becomes [0, 1, 2, 3, 4, -1, -1, ..., -1, 5, 6, 7]

  // check 0, 1, 2, 3, 4
  for (int i = 0; i < 5; ++i) {
    assert(test[i] == i); 
  }

  // check -1s
  for (int i = 5; i < 15; ++i) {
    assert(test[i] == -1);
  }

  // check 5, 6, 7
  for (int i = 0; i < 3; ++i) {
    assert(test[15 + i] == 5 + i);
  }

  test.Clear();

  // === test only having to shift the non-wrapped-around part, with the head
  //     being at index 0 === //

  // shift the head index a bit
  for (int i = 0; i < 5; ++i) {
    test.PushBack(0);
    test.PopFront();
  }

  for (int i = 0; i < 5; ++i) {
    test.PushBack(i);
  }

  // underlying array is now [3, 4, -, -, -, 0, 1, 2]

  test.Insert(95, 3);

  // underlying array is now [95, 3, 4, -, -, 0, 1, 2]

  test.Insert(96, 5);

  // underlying array is now [95, 3, 96, 4, -, 0, 1, 2]

  test.Insert(97, 7);

  // underlying array is now [95, 3, 96, 4, 97, 0, 1, 2]

  test.Insert(98, 4);

  // after resize, underlying array is now [0, 1, 2, 95, 98, 3, 96, 4, 97]
  assert(test[0] == 0);
  assert(test[1] == 1);
  assert(test[2] == 2);
  assert(test[3] == 95);
  assert(test[4] == 98);
  assert(test[5] == 3);
  assert(test[6] == 96);
  assert(test[7] == 4);
  assert(test[8] == 97);

  test.Clear();

  // === test shifting both wrapped and non-wrapped parts === //

  // shift the head index a bit
  for (int i = 0; i < 5; ++i) {
    test.PushBack(0);
    test.PopFront();
  }

  for (int i = 0; i < 4; ++i) {
    test.PushBack(i);
  }

  // underlying array is now [3, -, -, -, -, 0, 1, 2]

  test.Insert(95, 1);

  // underlying array is now [2, 3, -, -, -, 0, 95, 1]

  test.Insert(96, 0);

  // underlying array is now [1, 2, 3, -, -, 96, 0, 95]

  test.Insert(97, 2);
  
  // underlying array is now [95, 1, 2, 3, -, 96, 0, 97]
  
  test.Insert(98, 4);

  // underlying array is now [95, 98, 1, 2, 3, 96, 0, 97]

  test.Insert(99, 8);

  // after resize, underlying array becomes
  // [96, 0, 97, 95, 98, 1, 2, 3, 99]
  
  assert(test[0] == 96);
  assert(test[1] == 0);
  assert(test[2] == 97);
  assert(test[3] == 95);
  assert(test[4] == 98);
  assert(test[5] == 1);
  assert(test[6] == 2);
  assert(test[7] == 3);
  assert(test[8] == 99);

  test.Clear();
}


void testRandomized() {
  ReseedRand();
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

    } else if (50 <= operation && operation < 75) {
      int rand_val = RandInt(-10000, 100000);
      int rand_idx = RandInt(0, correct_deque.size());
      correct_deque.insert(correct_deque.begin() + rand_idx, rand_val);
      test_deque.Insert(rand_val, rand_idx);

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
      if (correct_deque[i] != test_deque[i]) {
        PrintDeque(test_deque);
      }
      assert(correct_deque[i] == test_deque[i]);
    }
  }
}


void testCopy() {
  Deque<int>* original = new Deque<int>;
  int num_elems = 456;
  for (int i = 0; i < num_elems; ++i) {
    original->PushBack(i);
  }

  Deque<int> stack_copy_construct = *original;
  assert(stack_copy_construct.Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_construct[i] == i);
  }

  Deque<int> stack_copy_assign;
  for (int i = 0; i < 25; ++i) {
    stack_copy_assign.PushBack(-123);
  }
  stack_copy_assign = *original;
  assert(stack_copy_assign.Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_assign[i] == i);
  }

  Deque<int>* heap_copy_construct = new Deque<int>(*original);
  assert(heap_copy_construct->Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert((*heap_copy_construct)[i] == i);
  }

  Deque<int>* heap_copy_assign = new Deque<int>;
  for (int i = 0; i < 25; ++i) {
    heap_copy_assign->PushBack(-123);
  }
  *heap_copy_assign = *original;
  assert(heap_copy_assign->Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert((*heap_copy_assign)[i] == i);
  }

  // check that deleting a copy doesn't delete the original
  delete heap_copy_assign;

  for (int i = 0; i < num_elems; ++i) {
    assert((*original)[i] == i); 
  }

  heap_copy_assign = new Deque<int>;
  *heap_copy_assign = *original;

  // check that deleting the original doesn't delete the copies
  delete original;
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_construct[i] == i);
    assert(stack_copy_assign[i] == i);
    assert((*heap_copy_construct)[i] == i);
    assert((*heap_copy_assign)[i] == i);
  }

  delete heap_copy_construct;
  delete heap_copy_assign;
}


int main() {
  testPushBack();
  testPushBackWithPopFront();
  testPushFront();
  testErase();
  testInsert();
  testRandomized();
  testCopy();
}

