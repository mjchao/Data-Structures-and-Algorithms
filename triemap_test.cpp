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
}


void testPutAndGetRandomized() {
  int num_inserts = 1000;
  std::vector<std::string> rand_strs = RandStrs(1, 100, num_inserts);
  Triemap<std::string, int> test;
  std::unordered_map<std::string, int> correct;
  for (int i = 0; i < num_inserts; ++i) {
    test.Put(rand_strs[i], i);
  }
  for (auto kv : correct) {
    assert(*test.Get(kv.first) == kv.second);
  }
}


void testRemove() {
  Triemap<std::string, int> test;
  test.Put("", 1);
  test.Remove("");
  assert(test.Get("") == nullptr);
}


int main() {
  ReseedRand();
  testPutAndGet();
  testPutAndGetRandomized();
  //testRemove();
  return 0;
}

