#include "Vector.h"
#include <assert.h>
#include <iostream>


using namespace dsalgo;


void testPushBack() {
  Vector<int> test;
  for (int i = 0; i < 1028; ++i) {
    test.PushBack(i);
    for (int j = 0; j <= i; ++j) {
      assert(test[j] == j);
    }
  }

  // destructor is implicitly tested here.
}


void testAssignment() {
  int num_inserts = 5014;
  Vector<int> test;
  for (int i = 0; i < num_inserts; ++i) {
    test.PushBack(i);
  }

  // reassign all elements
  for (int i = 0; i < num_inserts; ++i) {
    test[i] = -1 * i;
  }
  for (int i = 0; i < num_inserts; ++i) {
    assert(test[i] == -1 * i);
  }
}


void testCopy() {
  int num_inserts = 1028;

  Vector<int>* test = new Vector<int>;
  for (int i = 0; i < num_inserts; ++i) {
    test->PushBack(i);
  }

  Vector<int> testCopyConstructor = *test;
  for (int i = 0; i < num_inserts; ++i) {
    assert(testCopyConstructor[i] == i);
  }

  Vector<int> testCopyOperator;
  testCopyOperator.PushBack(125);
  testCopyOperator = *test;
  for (int i = 0; i < num_inserts; ++i) {
    assert(testCopyOperator[i] == i);
  }

  for (int i = 0; i < num_inserts; ++i) {
    (*test)[i] = -1 * i;
  }
  delete test;
  test = nullptr;

  for (int i = 0; i < num_inserts; ++i) {
    assert(testCopyConstructor[i] == i);
    assert(testCopyConstructor[i] == testCopyOperator[i]);
  }
}

void testMove() {
  int num_inserts = 1028;
  Vector<int> test;
  for (int i = 0; i < num_inserts; ++i) {
    test.PushBack(i);
  }

  Vector<int> testMoveConstructor = std::move(test);
  for (int i = 0; i < num_inserts; ++i) {
    assert(testMoveConstructor[i] == i);
  }

  Vector<int> testMoveOperator;
  testMoveOperator.PushBack(-999);
  testMoveOperator = std::move(testMoveConstructor);
  for (int i = 0; i < num_inserts; ++i) {
    assert(testMoveOperator[i] == i); 
  }
}


void testErase() {
  int num_inserts = 1028;

  Vector<int> test;
  for (int i = 0; i < num_inserts; ++i) {
    test.PushBack(i);
  }

  // erase 0, 2, 4, 6, ...
  for (int i = 0; i < test.Size(); ++i) {
    assert(test.Erase(i) == 2*i);
  }

  for (int i = 0; i < test.Size(); ++i) {
    assert(test[i] == 2*i + 1);
  }

  // erase the rest
  int remaining_size = test.Size();
  for (int i = 0 ; i < remaining_size; ++i) {
    assert(test[0] == 2*i + 1);
    assert(test.Erase(0) == 2*i + 1);
  }
}


void testPopFrontBack() {
  Vector<int> test;
  test.PushBack(1);
  test.PushBack(2);
  test.PushBack(3);
  assert(test.PopBack() == 3);
  assert(test.PopFront() == 1);
  assert(test.PopFront() == 2);
}


void testInsert() {
  int num_inserts = 157;
  Vector<int> test;
  for (int i = num_inserts - 1; i >= 0; --i) {
    test.Insert(i, 0);
  }
  for (int  i = 0; i < num_inserts; ++i) {
    assert(test[i] == i);
  }

  test.Clear();
  for (int i = 0; i < num_inserts; ++i) {
    test.PushBack(145);
  }
  for (int i = 0; i < num_inserts; ++i) {
    test.Insert(i, 2*i);
  }
  for (int i = 0; i < num_inserts; ++i) {
    assert(test[2*i] == i);
    assert(test[2*i+1] == 145);
  }
}


void testPushBackStr() {
  int num_inserts = 1035;
  Vector<std::string> test;
  for (int i = 0; i < num_inserts; ++i) {
    test.PushBack("abcdefg");
  }
  for (int i = 0; i < num_inserts; ++i) {
    assert(test[i] == "abcdefg");
  }
}


int main() {
  testPushBack();
  testCopy();
  testMove();
  testErase();
  testPopFrontBack();
  testInsert();
  testPushBackStr();
  return 0;
}

