#pragma once

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
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


int RandInt(int min, int max) {
  int range_size = (max - min) + 1;
  return min + (rand() % range_size);
}


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
  for (int i = 0; i < n; ++i) {
    rtn.push_back(RandInt(min, max));
  }
  return rtn;
}


/**
 * Generates a string with of random length with random characters. The
 * characters will be letters from 'a' to 'z'.
 *
 * @param min_len Minimum length of the string
 * @param max_len Maximum length of the string
 */
std::string RandStr(int min_len, int max_len) {
  int len = RandInt(min_len, max_len);
  std::vector<int> char_offsets = RandN(0, 25, len);
  std::vector<char> str_chars;
  std::transform(char_offsets.begin(), char_offsets.end(),
      std::back_inserter(str_chars),
      [](int offset) -> char {return 'a' + offset;});
  return std::string(str_chars.begin(), str_chars.end());
}


/**
 * Generates n random strings of random lengths. The strings will be ascii
 * consisting of letters from 'a' to 'z'.
 *
 * @param min_len Minimum length of each string
 * @param max_len Maximum length of each string
 * @param n Number of strings to generate
 * @return List of n random strings.
 */
std::vector<std::string> RandStrs(int min_len, int max_len, int n) {
  std::vector<int> str_lens = RandN(min_len, max_len, n);
  std::vector<std::string> rand_strs;
  for (int i = 0; i < n; ++i) {
    rand_strs.push_back(RandStr(min_len, max_len));
  }
  return rand_strs;
}


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

