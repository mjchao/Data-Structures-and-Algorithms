#include "Hashmap.h"
#include <assert.h>
#include <iostream>
#include <string>


using namespace dsalgo;


void testPutAndGet() {

  // test hashmap with full load factor
  Hashmap<std::string, int> test(8, 1.0);

  int num_elems = 128;
  for (int i = 0; i < num_elems; ++i) {
    test.Put(std::to_string(i), i);
    assert(test.Size() == i + 1);
    for (int j = 0; j < i; ++j) {
      assert(*test.Get(std::to_string(j)) == j);
    }
  }

  // test hashmap with recommended load factor
  Hashmap<std::string, int> test_rec_lf;
  for (int i = 0; i < num_elems; ++i) {
    test_rec_lf.Put(std::to_string(i), i);
    assert(test_rec_lf.Size() == i + 1);
    for (int j = 0; j < i; ++j) {
      assert(*test_rec_lf.Get(std::to_string(j)) == j);
    }
  }

  // test hashmap with very small load factor
  Hashmap<std::string, int> test_small_lf(8, 0.001);
  for (int i = 0; i < num_elems; ++i) {
    test_small_lf.Put(std::to_string(i), i);
    assert(test_small_lf.Size() == i + 1);
    for (int j = 0; j < i; ++j) {
      assert(*test_small_lf.Get(std::to_string(j)) == j);
    }
  }
}


void testRemove() {
  Hashmap<std::string, int> test;

  int num_elems = 128;
  for (int i = 0; i < num_elems; ++i) {
    test.Put(std::to_string(i), i);
    if (i % 16 == 0) {
      for (int j = 0; j < i; ++j) {
        assert(test.Remove(std::to_string(j)) == true);
      }
      for (int j = 0; j < i; ++j) {
        test.Put(std::to_string(j), j);
      }
    }
  }
  for (int i = 0; i < num_elems; ++i) {
    assert(test[std::to_string(i)] == i);
  }
}


int main() {
  testPutAndGet();
  testRemove();
  return 0;
}

