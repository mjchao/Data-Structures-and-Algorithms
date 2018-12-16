#include "ShmQueue.h"
#include <assert.h>


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

  ShmQueueHandle handle = test.NewHandle();
  char buf[3];
  int status;
  for (int i = 0; i < 3; ++i) {
    test.Enqueue("abc", 3);
  }
  for (int i = 0; i < 3; ++i) {
    assert(test.Dequeue(&handle, buf, 3, status) == 3);
    assert(status == ShmQueueStatus::OK);
    assert(std::string(buf) == "abc");
  }
}


int main() {
  testInitialize();
  testEnqueueDequeSingleProcess();
  return 0;
}

