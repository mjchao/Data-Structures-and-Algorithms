#include "Profiling.h"
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace dsalgo;


int main() {
  int mmapFd = open("test.shm", O_CREAT|O_RDWR, 00666);
  ftruncate(mmapFd, sizeof(int)*128);
  int* test_ptr = (int*) mmap(NULL, sizeof(int)*128, PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);
  std::cout << "Address of test_ptr: " << test_ptr << std::endl;
  int& test = *test_ptr;
  test = 123;
  std::cout << "Address of test: " << &test << std::endl;
  int64_t last_read = 0;
  while (true) {
    int64_t now = Clock::Now();
    if (now - last_read >= 1000000000) {
      std::cout << "test = " << test << std::endl;
      last_read = now;
    }
  }
  return 0;
}

