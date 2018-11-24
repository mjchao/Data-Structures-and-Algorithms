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
   * to val. end is returned if everything in the range is less than val.
   */
  template<typename Iterator, typename T>
  Iterator LowerBound(Iterator begin, Iterator end, const T& val) {
    int range_size = end - begin;

    // the element at index low must always be less than val, or it must be
    // the first element >= to val.
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
   * Performs a binary search on the last element == val. If no element equals
   * val, then the first element greater than val is returned.
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
   * @return iterator/pointer to the last element greater than val or to the
   * first element greater than val if val is not in the range. end is returned
   * if everything in the range is less than val.
   */
  template<typename Iterator, typename T>
  Iterator LowerBoundLast(Iterator begin, Iterator end, const T& val) {
    int range_size = end - begin;

    // the element at index low must be <= val, with the exception of index 0.
    // If index 0 is the first element greater than val, the algorithm will
    // correctly choose index 0 as the answer.
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

        // mid == val, so setting low to mid just brings the lower bound higher
        low = mid;
      }

      // recompute mid after updating the lower/upper bound of the search range
      mid = (low + high) / 2;
    }

    return begin + mid;
  }


  /**
   * Performs a binary search on the first element greater than val.
   *
   * The pointers must be to a random-access range.
   *
   * Elements in the range should be < comparable.
   *
   * @param begin iterator to the beginning of the sorted range to search
   * @param end iterator to the end of the sorted range to search
   * @param val the value to search for
   * @return iterator to the first element greater than val. end is returned
   * if all elements are less than val.
   */
  template<typename Iterator, typename T>
  Iterator UpperBound(Iterator begin, Iterator end, const T& val) {
    int range_size = end - begin;

    // low must be the index on or before the first element greater than val.
    // so either the element at index low is <= val, or the element at index
    // low is > val and it is the first element greater than val.
    int low = 0;

    // element at index high must be > val.
    int high = range_size;

    // mid is the element to test.
    int mid = (low + high) / 2;

    // when the binary search ends, we'll have low = mid = high

    while (low < high) {
      const T& test = *(begin + mid);

      if (test <= val) {

        // if element at mid is <= val, then the element at mid + 1 must also
        // be <= val or it must be the 
        low = mid + 1;
      } else /* (test > val) */ {

        // if test > val, then setting high = mid still guarantees that
        // the element at high is greater than val.
        high = mid;
      }

      // recompute mid after updating the lower/upper bound of the search range
      mid = (low + high) / 2;
    }

    return begin + mid;
  }
} // namespace dsalgo

