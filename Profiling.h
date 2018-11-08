#pragma once

#include <chrono>
#include <cstdlib>
#include <vector>

namespace dsalgo {

/**
 * Provides clock utilities for profiling.
 */
class Clock {

public:
  
  /**
   * @return Current time in nanoseconds.
   */
  static int64_t Now() {
    return std::chrono::high_resolution_clock::now().time_since_epoch() /
      std::chrono::nanoseconds(1);
  }
};

/**
 * Generates n random numbers between min and max, inclusive.
 *
 * @param min min value of random numbers
 * @param max max value of random numbers
 * @param n number of random numbers to generate
 * @return List of n random numbers in the range [min, max]
 */
std::vector<int> RandN(int min, int max, int n) {
  std::vector<int> rtn;
  int range_size = (max - min) + 1;
  for (int i = 0; i < n; ++i) {
    rtn.push_back(min + (rand() % range_size)); 
  }
  return rtn;
}

/**
 * Given that a program made num_calls and took total_time, computes the time
 * per call (total_time / num_calls).
 */
double ComputeTimePerCall(int64_t total_time_ns, int num_calls) {
  return static_cast<double>(total_time_ns) / num_calls;
}

} // namespace dsalgo
