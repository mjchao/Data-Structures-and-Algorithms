#include "ShmQueue.h"


using namespace dsalgo;


void testInitialize() {
  ShmQueue test("test.shm", 4096, true);
  ShmQueue test2("test.shm");
}


int main() {
  testInitialize();
  return 0;
}

