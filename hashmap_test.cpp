#include "Hashmap.h"
#include "Random.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
      assert(test[std::to_string(j)] == j);
    }
  }

  // test hashmap with recommended load factor
  Hashmap<std::string, int> test_rec_lf;
  for (int i = 0; i < num_elems; ++i) {
    test_rec_lf.Put(std::to_string(i), i);
    assert(test_rec_lf.Size() == i + 1);
    for (int j = 0; j < i; ++j) {
      assert(*test_rec_lf.Get(std::to_string(j)) == j);
      assert(test_rec_lf[std::to_string(j)] == j);
    }
  }

  // test hashmap with very small load factor
  Hashmap<std::string, int> test_small_lf(8, 0.001);
  for (int i = 0; i < num_elems; ++i) {
    test_small_lf.Put(std::to_string(i), i);
    assert(test_small_lf.Size() == i + 1);
    for (int j = 0; j < i; ++j) {
      assert(*test_small_lf.Get(std::to_string(j)) == j);
      assert(test_small_lf[std::to_string(j)] == j);
    }
  }
}


void testInsertViaOperator() {
  Hashmap<std::string, int> test_default_insertion;
  int num_elems = 234;
  for (int i = 0; i < num_elems; ++i) {
    test_default_insertion[std::to_string(i)];
  }
  assert(test_default_insertion.Size() == num_elems);

  Hashmap<std::string, int> test;
  for (int i = 0; i < num_elems; ++i) {
    test[std::to_string(i)] = i;
    for (int j = 0; j <= i; ++j) {
      assert(test[std::to_string(j)] == j);
    }
  }
}


void testRemoveAndGet() {
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
    assert(test.Size() == i + 1);
  }
  for (int i = 0; i < num_elems; ++i) {
    assert(*test.Get(std::to_string(i)) == i);
    assert(test[std::to_string(i)] == i);
  }
  for (int i = -1000; i < -500; ++i) {
    assert(test.Get(std::to_string(i)) == nullptr);
  }
}

void testCopy() {
  Hashmap<std::string, int>* original = new Hashmap<std::string, int>;

  int num_elems = 456;
  for (int i = 0; i < num_elems; ++i) {
    original->Put(std::to_string(i), i);
  }

  // test copy constructor on stack-alloc'ed mem
  Hashmap<std::string, int> stack_copy_construct = *original;
  assert(stack_copy_construct.Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_construct[std::to_string(i)] == i); 
  }

  // test copy assign op on stack-alloc'ed mem
  Hashmap<std::string, int> stack_copy_assign;
  for (int i = 0; i < 127; ++i) {
    stack_copy_assign.Put(std::to_string(i), i);
  }
  stack_copy_assign = *original;
  assert(stack_copy_assign.Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_assign[std::to_string(i)] == i);
  }

  // test copy constructor on heap-alloc'ed mem
  Hashmap<std::string, int>* heap_copy_construct =
      new Hashmap<std::string, int>(*original);
  assert(heap_copy_construct->Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert((*heap_copy_construct)[std::to_string(i)] == i);
  }

  // test copy assign op on heap-alloc'ed mem
  Hashmap<std::string, int>* heap_copy_assign = new Hashmap<std::string, int>;
  for (int i = 0; i < 127; ++i) {
    heap_copy_assign->Put(std::to_string(i), i);
  }
  *heap_copy_assign = *original;
  assert(heap_copy_assign->Size() == num_elems);
  for (int i = 0; i < num_elems; ++i) {
    assert((*heap_copy_assign)[std::to_string(i)] == i);
  }

  // check that editing and deleting a copy doesn't delete the original
  for (int i = 0; i < 256; ++i) {
    heap_copy_assign->Remove(std::to_string(i));
  }
  delete heap_copy_assign;

  for (int i = 0; i < num_elems; ++i) {
    assert((*original)[std::to_string(i)] == i);
  }

  heap_copy_assign = new Hashmap<std::string, int>;
  *heap_copy_assign = *original;

  // check that editing and deleting the original doesn't affect copies
  for (int i = 0; i < 300; ++i) {
    original->Remove(std::to_string(i));
  }
  delete original;
  for (int i = 0; i < num_elems; ++i) {
    assert(stack_copy_construct[std::to_string(i)] == i);
    assert(stack_copy_assign[std::to_string(i)] == i);
    assert((*heap_copy_construct)[std::to_string(i)] == i);
    assert((*heap_copy_assign)[std::to_string(i)] == i);
  }

  delete heap_copy_construct;
  delete heap_copy_assign;
}

void testMove() {
  Hashmap<std::string, int> test;
  int num_elems = 337;
  for (int i = 0; i < num_elems; ++i) {
    test.Put(std::to_string(i), i);
  }

  Hashmap<std::string, int> test_move_construct = std::move(test);
  for (int i = 0; i < num_elems; ++i) {
    assert(test_move_construct[std::to_string(i)] == i);
  }

  Hashmap<std::string, int> test_move_op;
  test_move_op["-123"] = -123;
  test_move_op = std::move(test_move_construct);
  for (int i = 0; i < num_elems; ++i) {
    assert(test_move_op[std::to_string(i)] == i);
  }
  assert(test_move_op.Get("-123") == nullptr);
}


void testRandomized(Hashmap<std::string, int>& test_map, int num_ops) {
  std::unordered_map<std::string, int> correct_map;

  for (int i = 0; i < num_ops; ++i) {
    int operation = RandInt(0, 75);

    // insert via Put
    if (0 <= operation && operation < 25) {
      std::string rand_key = RandStr(5, 10);
      int rand_val = RandInt(-100000, 100000);
      correct_map[rand_key] = rand_val;
      test_map.Put(rand_key, rand_val);

    // insert via operator[]
    } else if (25 <= operation && operation < 50) {
      std::string rand_key = RandStr(5, 10);
      int rand_val = RandInt(-100000, 100000);
      correct_map[rand_key] = rand_val;
      test_map[rand_key] = rand_val;

    // remove a key
    } else if (50 <= operation && operation < 75) {
      if (!correct_map.empty()) {
        std::string key_to_remove = correct_map.begin()->first;
        correct_map.erase(key_to_remove);
        assert(test_map.Remove(key_to_remove) == true);
      }
    }

    for (const auto& entry : correct_map) {
      assert(test_map[entry.first] == entry.second);
    }
    assert(test_map.Size() == static_cast<int>(correct_map.size()));
    for (const auto& entry : correct_map) {
      assert(*test_map.Get(entry.first) == entry.second);
    }
  }
}

void testRandomized() {
  ReseedRand();

  // use recommended load factor
  Hashmap<std::string, int> test;
  testRandomized(test, 5000);

  // use high load factor
  test = Hashmap<std::string, int>(8, 1);
  testRandomized(test, 1000);

  // use small load factor
  test = Hashmap<std::string, int>(8, 0.01);
  testRandomized(test, 5000);
}


int main() {
  testPutAndGet();
  testInsertViaOperator();
  testRemoveAndGet();
  testCopy();
  testMove();
  testRandomized();
  return 0;
}

