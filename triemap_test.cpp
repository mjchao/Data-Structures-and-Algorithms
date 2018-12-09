#include "Triemap.h"
#include "Random.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>


using namespace dsalgo;


void testPutAndGet() {
  Triemap<std::string, int> test;
  test.Put("abc", 1);
  assert(*test.Get("abc") == 1);
  assert(test.Get("ab") == nullptr);

  test.Put("", 2);
  assert(*test.Get("") == 2);
  assert(*test.Get("abc") == 1);
  assert(test.Get("ab") == nullptr);
  assert(test.Size() == 2);
}


void testPutAndGetRandomized() {
  int num_inserts = 1000;
  std::vector<std::string> rand_strs = RandStrs(1, 100, num_inserts);
  Triemap<std::string, int> test;
  std::unordered_map<std::string, int> correct;
  for (int i = 0; i < num_inserts; ++i) {
    test.Put(rand_strs[i], i);
    correct[rand_strs[i]] = i;
  }
  for (auto kv : correct) {
    assert(*test.Get(kv.first) == kv.second);
  }
  assert(test.Size() == static_cast<int>(correct.size()));
}


void testRemove() {
  Triemap<std::string, int> test;
  test.Put("", 1);
  assert(test.Remove("") == true);
  assert(test.Get("") == nullptr);
  assert(test.Size() == 0);

  test.Put("abcdefg", 2);
  assert(test.Remove("a") == false);
  assert(test.Remove("abc") == false);
  assert(test.Remove("abcdef") == false);
  assert(test.Remove("abcdefg") == true);
  assert(test.Get("abcdefg") == nullptr);
  assert(test.Size() == 0);

  test.Put("abcdefg", 3);
  test.Put("abc", 4);
  assert(test.Remove("abc") == true);
  assert(test.Get("abc") == nullptr);
  assert(test.Size() == 1);
  assert(*test.Get("abcdefg") == 3);
  assert(test.Remove("abcdefg") == true);
  assert(test.Get("abcdefg") == nullptr);
  assert(test.Size() == 0);
}


void testRemoveRandomized() {
  int num_inserts = 1000;
  std::vector<std::string> rand_strs = RandStrs(1, 100, num_inserts);
  Triemap<std::string, int> test;
  std::unordered_map<std::string, int> correct;
  for (int i = 0; i < num_inserts; ++i) {
    test.Put(rand_strs[i], i);
    correct[rand_strs[i]] = i;
  }
  for (const std::string& s : rand_strs) {
    bool should_remove = (RandInt(0, 1) == 0);
    if (should_remove) {
      test.Remove(s);
      correct.erase(correct.find(s));
    }
  }
  for (const std::string& s : rand_strs) {
    if (correct.find(s) != correct.end()) {
      assert(*test.Get(s) == correct[s]);    
    } else {
      assert(test.Get(s) == nullptr);
    }
  }
  assert(test.Size() == static_cast<int>(correct.size()));
}


void testCopy() {
  int num_elems = 128;
  Triemap<std::string, int>* original = new Triemap<std::string, int>;
  for (int i = 0; i < num_elems; ++i) {
    original->Put(std::to_string(i), i);
  }

  // test copy constructor
  Triemap<std::string, int>* copy_construct = new Triemap<std::string, int>;
  copy_construct->Put("blah", 1);
  copy_construct->Put("1", 999);
  *copy_construct = Triemap<std::string, int>(*original);
  for (int i = 0; i < num_elems; ++i) {
    assert(*copy_construct->Get(std::to_string(i)) == i);
  }
  assert(copy_construct->Size() == num_elems);

  // test copy assignment operator
  Triemap<std::string, int>* copy_assign =
      new Triemap<std::string, int>;
  copy_assign->Put("blah", 1);
  copy_assign->Put("1", 999);
  *copy_assign = *original;
  for (int i = 0; i < num_elems; ++i) {
    assert(*copy_assign->Get(std::to_string(i)) == i);
  }
  assert(copy_assign->Size() == num_elems);


  // test deleting original does not affect copies
  for (int i = 0; i < num_elems; ++i) {
    original->Remove(std::to_string(i));
  }
  delete original;

  for (int i = 0; i < num_elems; ++i) {
    assert(*copy_construct->Get(std::to_string(i)) == i);
    assert(*copy_assign->Get(std::to_string(i)) == i);
  }
  assert(copy_construct->Size() == num_elems);
  assert(copy_assign->Size() == num_elems);
  delete copy_construct;
  delete copy_assign;
}


void testMove() {
  int num_elems = 128;
  Triemap<std::string, int>* original = new Triemap<std::string, int>;
  for (int i = 0; i < num_elems; ++i) {
    original->Put(std::to_string(i), i);
  }

  Triemap<std::string, int>* move_construct = new Triemap<std::string, int>;
  move_construct->Put("blah", 1);
  move_construct->Put("1", 999);
  *move_construct = Triemap<std::string, int>(std::move(*original));
  delete original;
  for (int i = 0; i < num_elems; ++i) {
    assert(*move_construct->Get(std::to_string(i)) == i);
  }
  assert(move_construct->Size() == num_elems);

  Triemap<std::string, int>* move_assign = new Triemap<std::string, int>;
  move_assign->Put("blah", 1);
  move_assign->Put("1", 999);
  *move_assign = std::move(*move_construct);
  delete move_construct;
  for (int i = 0; i < num_elems; ++i) {
    assert(*move_assign->Get(std::to_string(i)) == i);
  }
  assert(move_assign->Size() == num_elems);
  delete move_assign;
}


int main() {
  ReseedRand();
  testPutAndGet();
  testPutAndGetRandomized();
  testRemove();
  testCopy();
  testMove();
  return 0;
}

