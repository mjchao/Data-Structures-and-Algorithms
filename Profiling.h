#pragma once

#include <chrono>
#include <iostream>
#include <string>

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
 * Given that a program made num_calls and took total_time, computes the time
 * per call (total_time / num_calls).
 */
double ComputeTimePerCall(int64_t total_time_ns, int num_calls) {
  return static_cast<double>(total_time_ns) / num_calls;
}


/**
 * Prints total time, total calls, and ns/call info for profiling.
 *
 * @param total_time_ns Total time spent running
 * @param num_calls Number of calls made to a function
 * @param prefix String to print in front of each line of stats. Useful
 *  if you need to indent the stats to organize them.
 */
void PrintStats(int64_t total_time_ns, int num_calls,
    std::string output_prefix="") {
  std::cout << output_prefix << "Total Time: " << total_time_ns << std::endl;
  std::cout << output_prefix << "Total Calls: " << num_calls << std::endl;
  std::cout << output_prefix << "ns/call: "
    << ComputeTimePerCall(total_time_ns, num_calls) << std::endl;
}

} // namespace dsalgo

