#include "Triemap.h"
#include <assert.h>
#include <iostream>
#include <string>


using namespace dsalgo;


void testPutAndGet() {
  Triemap<std::string, int> test;
  test.Put("abc", 1);
  assert(*test.Get("abc") == 1);
  assert(test.Get("ab") == nullptr);
}

int main() {
  testPutAndGet();
  return 0;
}

