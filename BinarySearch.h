#pragma once
#include <iostream>

namespace dsalgo {

  /**
   * Performs a binary search for the first element >= val.
   *
   * The pointers must be to a random-access range.
   *
   * Elements in the range should be < comparable.
   *
   * @param first iterator/pointer to the first element in a sorted range to
   * search
   * @param last iterator/pointer to one after the last element in a sorted
   * range to search
   * @param val the element to search for.
   * @return iterator/pointer to the first element that is greater than or equal
   * to val.
   */
  template<typename Iterator, typename T>
  Iterator LowerBound(Iterator begin, Iterator end, const T& val) {
    int range_size = end - begin;

    // the element at index low must always be less than val, or it must be
    // the first element equal to val.
    int low = 0;

    // the element at index high must be >= val.
    int high = range_size;

    // when the search ends, we'll have low = mid = high.

    // mid is the element to test.
    int mid = (low + high) / 2;

    while (low < high) {
      const T& test = *(begin + mid);

      // too low
      if (test < val) {

        // next element after mid must be <= val. If it is < val, we're okay.
        // If it is == val, then it is the first element equal to val and we
        // haven't violated the lower bound.
        low = mid + 1; 

      // too high
      } else /* (test >= val) */{

        // mid is >= val, which always satisfies the upper bound.
        high = mid;
      }

      // recompute mid after updating the lower/upper bound of the search range
      mid = (low + high) / 2;
    }

    return begin + mid;
  }


  /**
   * Performs a binary search on the last element <= val.
   *
   * The pointers must be to a random-access range.
   *
   * Elements in the range should be < comparable.
   *
   * @param first iterator/pointer to the first element in a sorted range to
   * search
   * @param last iterator/pointer to one after the last element in a sorted
   * range to search
   * @param val the element to search for.
   * @return iterator/pointer to the first element that is greater than or equal
   * to val.
   */
  template<typename Iterator, typename T>
  Iterator LowerBoundLast(Iterator begin, Iterator end, const T& val) {
    int range_size = end - begin;

    // the element at index low must be <= val.
    int low = 0;

    // the element at index high must be > val.
    int high = range_size;

    // when the search ends, we'll have low = mid and high = mid + 1.

    // mid is the element to test.
    int mid = (low + high) / 2;

    while (low < mid) {
      const T& test = *(begin + mid);

      if (test < val) {

        // mid is < val, so setting low + 1 to mid will maintain the lower
        // bound restriction
        low = mid + 1; 

      } else if (val < test) {

        // mid is > val, so setting high to mid will maintain the lower bound
        // restriction
        high = mid;

      } else /* (test == val) */ {

        // mid == val, so setting low to mid just bring the lower bound higher
        low = mid;
      }

      // recompute mid after updating the lower/upper bound of the search range
      mid = (low + high) / 2;
    }

    return begin + mid;
  }

} // namespace dsalgo

