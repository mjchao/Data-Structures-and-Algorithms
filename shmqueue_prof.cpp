#include "ShmQueue.h"
#include "Random.h"
#include "Profiling.h"

#include <string>
#include <unistd.h>
#include <vector>

using namespace dsalgo;


void ProfileSingleProducerMultiConsumer() {
  constexpr int MIN_STR_LEN = 800;
  constexpr int MAX_STR_LEN = 1000;
  constexpr int SHM_SIZE = 8192;
  constexpr int N_STRS = 1000;
  constexpr int N_CONSUMERS = 7;
  constexpr int DELAY_BETWEEN_STRS_US = 100;

  std::vector<std::string> rand_strs = RandStrs(
      MIN_STR_LEN, MAX_STR_LEN, N_STRS);
  int total_str_bytes = 0;
  std::vector<const char*> rand_cstrs;
  for (const std::string& s : rand_strs) {
    rand_cstrs.push_back(s.c_str());
    total_str_bytes += s.size();
  }

  int proc_idx = 0;
  while (proc_idx < N_CONSUMERS) {
    int pid = fork();
    // child continues spawning off new threads
    if (pid == 0) {
      proc_idx += 1;

      // child should continue to spawn additional processes and not break

    } else {

      // if the parent has spawned a child, its job is done and can stop
      break;
    }
  }

  // process index 0 is the producer, process index 1..N_CONSUMERS are the
  // consumers.
  if (proc_idx == 0) {
    ShmQueue test("prof.shm", SHM_SIZE, true);

    // sleep 1 second to give producers time to startup
    usleep(1000000);

    // produce data
    int64_t total_enqueue_time = 0;
    for (int i = 0; i < N_STRS; ++i) {
      int64_t start = Clock::Now();
      for (int j = 0; j < static_cast<int>(rand_strs[i].size()); ++j) {
        test.Enqueue(rand_cstrs[i] + j, 1);
      }
      int64_t stop = Clock::Now();
      total_enqueue_time += (stop - start);
      usleep(DELAY_BETWEEN_STRS_US);
    }

    // get stats from consumers that were put in the shared memory queue
    ShmQueueHandle handle = test.NewHandle();
    constexpr int num_consumer_stats_bytes = N_CONSUMERS *
        (sizeof(int) + sizeof(int64_t));
    char stats_from_consumers[num_consumer_stats_bytes];
    int stats_byte_idx = 0;
    int status;
    while (stats_byte_idx < num_consumer_stats_bytes) {
      int bytes_read = test.Dequeue(&handle,
          stats_from_consumers + stats_byte_idx,
          (num_consumer_stats_bytes - stats_byte_idx),
          status);
      stats_byte_idx += bytes_read;
    }

    char* stats_ptr = stats_from_consumers;
    for (int i = 0; i < N_CONSUMERS; ++i) {
      int consumer_idx = *reinterpret_cast<int*>(stats_ptr);
      stats_ptr += sizeof(int);

      int64_t consumer_dequeue_time = *reinterpret_cast<int64_t*>(stats_ptr);
      stats_ptr += sizeof(int64_t);

      std::cout << "=== ShmQueue Consumer #" << consumer_idx << " Stats ===\n";
      PrintStats(consumer_dequeue_time, total_str_bytes, "\t");
      std::cout << "\n\n\n";
    }

    std::cout << "=== ShmQueue Enqueue Stats ===\n";
    PrintStats(total_enqueue_time, total_str_bytes, "\t");
    std::cout << "\n\n\n";

  } else {

    // sleep to give the producer some time to set p the shared memory
    usleep(1000);
    ShmQueue test("prof.shm");
    ShmQueueHandle handle = test.NewHandle();

    char buf[MAX_STR_LEN];
    int buf_idx = 0;
    int status;
    int64_t total_dequeue_time = 0;
    for (int i = 0; i < N_STRS; ++i) {
      int bytes_left = static_cast<int>(rand_strs[i].size());
      while (bytes_left > 0) {

        int64_t start = Clock::Now();
        int bytes_read = test.Dequeue(
            &handle, buf + buf_idx, bytes_left, status);
        int64_t stop = Clock::Now();

        // only count the time if we read non-zero bytes. Shouldn't count
        // time spent waiting towards the dequeue time
        if (bytes_read > 0) {
          total_dequeue_time += (stop - start);
          bytes_left -= bytes_read;
        }

        // should not happen because we are throttling the enqueues by waiting
        // 100 microseconds between strings and the enqueued strings never
        // overflow the buffer
        if (status == ShmQueueStatus::EVICTED) {
          std::cerr << "Consumer " << proc_idx << " was evicted. "
              << "Please rerun." << std::endl;
          // hope that the profiling ends within 3 seconds
          usleep(3000000);
          i = N_STRS;
          break;
        }
      }
    }

    // give process 0 some time to set up a handle to read the stats from
    // shared memory.
    usleep(1000);

    // write the stats back into shared memory for process 0 to report
    char stats[sizeof(int) + sizeof(int64_t)];
    std::memcpy(stats, reinterpret_cast<char*>(&proc_idx), sizeof(int));
    std::memcpy(stats + sizeof(int),
        reinterpret_cast<char*>(&total_dequeue_time), sizeof(int64_t));
    test.Enqueue(stats, sizeof(int) + sizeof(int64_t));
  }
  
}


int main() {
  ReseedRand();
  ProfileSingleProducerMultiConsumer();
  return 0;
}

