#pragma once

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>


namespace dsalgo {

/**
 * Reseeds random number generator with current system time.
 */
void ReseedRand() {
  srand(time(NULL));
}


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

} // namespace dsalgo

