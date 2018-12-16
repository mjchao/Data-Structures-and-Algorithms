#include "ShmQueue.h"
#include "Random.h"
#include <assert.h>
#include <unistd.h>


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

  // put 9 bytes into queue and dequeue them
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

  // put 2 bytes into queue and try to dequeue 3 bytes
  test.Enqueue("yz", 2);
  assert(test.Dequeue(&handle, buf, 3, status) == 2);
  assert(status == ShmQueueStatus::TRUNCATED);
  // only first two bytes of buf should have changed
  assert(std::string(buf) == "yzc");

  // queue is now empty, so should get 0 bytes from dequeuing and buf should be
  // unchanged
  for (int i = 0; i < 3; ++i) {
    assert(test.Dequeue(&handle, buf, 12, status) == 0);
    assert(status == ShmQueueStatus::TRUNCATED);
    assert(std::string(buf) == "yzc");
  }

  // create second handle to test that both handles are independent of each
  // other.
  ShmQueueHandle handle2 = test.NewHandle();
  for (int i = 0; i < 4; ++i) {
    test.Enqueue("def", 3);
  }

  for (int i = 0; i < 4; ++i) {
    assert(test.Dequeue(&handle, buf, 3, status) == 3);
    assert(status == ShmQueueStatus::OK);
    assert(std::string(buf) == "def");
    assert(test.Dequeue(&handle2, buf, 3, status) == 3);
    assert(status == ShmQueueStatus::OK);
    assert(std::string(buf) == "def");
  }

  // get both handles evicted
  for (int i = 0; i < 16; ++i) {
    test.Enqueue("x", 1);
  }

  for (int i = 0; i < 3; ++i) {
    assert(test.Dequeue(&handle, buf, 3, status) == 0);
    assert(status == ShmQueueStatus::EVICTED);
    assert(std::string(buf) == "def");
    assert(test.Dequeue(&handle2, buf, 3, status) == 0);
    assert(status == ShmQueueStatus::EVICTED);
    assert(std::string(buf) == "def");
  }
}

// one process enqueues N characters, the remaining processes read the N
// characters.
void testSingleProducerMultiConsumer() {
  int N = 10000;
  int N_PROCS = 8;
  std::string rand_str = RandStr(N, N);
  std::vector<int> rand_sleep_intervals = RandN(1, 100, N); 

  int proc_idx = 0;
  while (proc_idx < N_PROCS) {
    int pid = fork(); 
    // child continues spawning off new threads
    if (pid == 0) {
      // this child's process index is now 1 greater than it's parent's index
      proc_idx += 1;

      // child should continue to spawn additional processes
    } else {
      // if the parent has spawned a child, it's job is done and can stop
      break;
    }
  }
  
}


int main() {
  testInitialize();
  testEnqueueDequeSingleProcess();
  return 0;
}

