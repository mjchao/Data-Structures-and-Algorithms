#include "Profiling.h"
#include <iostream>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace dsalgo;


int main(int argc, char** argv) {
  int mmapFd = open("test.shm", O_CREAT|O_RDWR, 00666);
  int* test_ptr = (int*) mmap(NULL, sizeof(int)*128, PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);
  int& test = *test_ptr;
  std::cout << "Writing to test: " << &test << std::endl;
  int64_t last_write = 0;
  while (true) {
    int64_t now = Clock::Now();
    if (now - last_write >= 1000000000) {
      std::cout << "Decremented test" << std::endl;
      test -= 1; 
      last_write = now;
    }
  }
  return 0;
}
