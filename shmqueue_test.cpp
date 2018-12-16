#include "ShmQueue.h"
#include "Profiling.h"
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
  constexpr int N = 10000;
  int N_CONSUMERS = 8;

  // all processes will be limited to 10 seconds of execution time.
  int64_t TIME_LIMIT_NS = 10000000000;
  int64_t start = Clock::Now();

  std::string rand_str = RandStr(N, N);
  const char* rand_cstr = rand_str.c_str();
  std::vector<int> rand_sleep_intervals = RandN(1, 20, N); 
  // prefer to have "bursty" periods during which we don't sleep.
  for (int i = 0; i < N; ++i) {
    if (rand_sleep_intervals[i] <= 15) {
      rand_sleep_intervals[i] = 0;
    }
  }

  int proc_idx = 0;

  while (proc_idx < N_CONSUMERS) {
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

  std::cout << proc_idx << std::endl;

  // process index 0 is the producer, process index 1..N_CONSUMERS are the
  // consumers.
  if (proc_idx == 0) {
    ShmQueue test("test.shm", 4096, true);
    // sleep 1 second to give producers some time to start dequeuing from
    // shared memory
    usleep(1000000);

    // produce data
    for (int i = 0; i < N; ++i) {
      test.Enqueue(rand_cstr + i, 1);
      usleep(rand_sleep_intervals[i]);
    }
    std::cout << "Done Writing" << std::endl;
  } else {

    // sleep to give the producer some time to set up the shared memory
    usleep(1000);
    ShmQueue test("test.shm");
    ShmQueueHandle handle = test.NewHandle();

    // consume
    char buf[N];
    int buf_idx = 0;
    int status;
    std::cout << "Start reading" << std::endl;
    while (Clock::Now() - start <= TIME_LIMIT_NS) {
      int bytes_read = test.Dequeue(&handle, buf + buf_idx, 1, status); 

      // in practice, the child processes should not get evicted since the
      // buffer is large.
      assert(status != ShmQueueStatus::EVICTED);
      buf_idx += bytes_read;
      
      // stop after we've read in everything
      if (buf_idx >= N) {
        break;
      }
    }
    assert(rand_str == buf);
  }
}


int main() {
  //testInitialize();
  //testEnqueueDequeSingleProcess();
  testSingleProducerMultiConsumer();
  return 0;
}

