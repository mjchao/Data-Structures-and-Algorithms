#pragma once
#define LIKELY(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

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

static inline bool IsPowerOf2(int x) {
	// https://stackoverflow.com/questions/108318/
	// whats-the-simplest-way-to-test-whether-a-
	// number-is-a-power-of-2-in-c
	return x > 0 && ((x & (x - 1)) == 0);
}

} // namespace dsalgo

