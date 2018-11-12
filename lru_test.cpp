#include "LruQueue.h"
#include <assert.h>
#include <iostream>
#include <vector>


using namespace dsalgo;


void testConstructor() {
  std::vector<int> elems;

  elems = {1};
  LruQueue<int> test(elems);
  assert(test.GetLru() == 1);

  elems = {1, 2, 3, 4, 5};
  LruQueue<int> test2(elems);
  assert(test.GetLru() == 1);

  // destructor is implicitly tested
}


void testCopy(const std::vector<int> elems) {
  assert(elems.size() > 0);

  LruQueue<int> stack_alloced_queue(elems);
  LruQueue<int> stack_alloced_copy(stack_alloced_queue);
  for (int i = 0; i < 5 * static_cast<int>(elems.size()) + 1; ++i) {
    stack_alloced_copy.MarkUsed(stack_alloced_copy.GetLru());
    assert(stack_alloced_copy.GetLru() == elems[(i + 1) % elems.size()]);
    assert(stack_alloced_queue.GetLru() == elems[0]);
  }

  stack_alloced_copy = stack_alloced_queue;
  for (int i = 0; i < 5 * static_cast<int>(elems.size()) + 1; ++i) {
    stack_alloced_copy.MarkUsed(stack_alloced_copy.GetLru());
    assert(stack_alloced_copy.GetLru() == elems[(i + 1) % elems.size()]);
    assert(stack_alloced_queue.GetLru() == elems[0]);
  }

  LruQueue<int>* heap_alloced_queue = new LruQueue<int>(elems);
  LruQueue<int> heap_alloced_copy(*heap_alloced_queue);
  delete heap_alloced_queue;
  for (int i = 0; i < 5 * static_cast<int>(elems.size()) + 1; ++i) {
    heap_alloced_copy.MarkUsed(heap_alloced_copy.GetLru());
    assert(heap_alloced_copy.GetLru() == elems[(i + 1) % elems.size()]);
  }

  heap_alloced_queue = new LruQueue<int>(elems);
  heap_alloced_copy = *heap_alloced_queue;
  delete heap_alloced_queue;
  for (int i = 0; i < 5 * static_cast<int>(elems.size()) + 1; ++i) {
    heap_alloced_copy.MarkUsed(heap_alloced_copy.GetLru());
    assert(heap_alloced_copy.GetLru() == elems[(i + 1) % elems.size()]);
  }
}


void testCopy() {
  testCopy({0});
  testCopy({0, 1});
  testCopy({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}


void testMarkUsed() {
  std::vector<int> elems = {0, 1, 2, 9, 3, 4, 5, 6, 7, 8};
  LruQueue<int> test(elems);
  // mark used something in the middle and also mark it as used many times
  // when it's on already most-recently used.
  for (int i = 0; i < 27; ++i) {
    test.MarkUsed(9);
  }

  for (int i = 0 ; i < 100; ++i) {
    assert(test.GetLru() == (i % 10));
    test.MarkUsed(test.GetLru());
  }
}

void testMarkUsedAlternating() {
  std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  LruQueue<int> test(elems);

  // alternate between marking 8 and 9 as used
  for (int i = 0; i < 24; ++i) {
    test.MarkUsed(8);
    test.MarkUsed(9);
  }

  for (int i = 0 ; i < 100; ++i) {
    assert(test.GetLru() == (i % 10));
    test.MarkUsed(test.GetLru());
  }
}


int main() {
  testConstructor();
  testCopy();
  testMarkUsed();
  testMarkUsedAlternating();
  return 0;
}

