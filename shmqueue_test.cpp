#include "ShmQueue.h"


using namespace dsalgo;


void testInitialize() {
  ShmQueue test("test.shm", 4096, true);
  ShmQueue test2("test.shm");
}

void testEnqueueDequeSingleProcess() {
  ShmQueue test("test.shm", 16, true);
  for (int i = 0; i < 10; ++i) {
    test.Enqueue("abc", 3);
  }
}


int main() {
  testInitialize();
  testEnqueueDequeSingleProcess();
  return 0;
}

