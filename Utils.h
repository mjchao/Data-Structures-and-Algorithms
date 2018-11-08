#pragma once

namespace dsalgo {

  static inline int NextPowerOf2(int x) {
		// https://stackoverflow.com/questions/466204/
		// rounding-up-to-next-power-of-2
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
  }
  
} // namespace dsalgo
