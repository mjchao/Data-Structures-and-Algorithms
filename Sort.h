#pragma once

#include <utility>
#include <iostream>
#include <vector>

namespace dsalgo {
namespace {

/**
 * Performs the partition step of the quicksort algorithm.
 *
 * @param begin the begin of a range to partition
 * @param end the end of a range to partition
 * @return iterator to the pivot
 */
template<typename Iterator>
Iterator QuicksortPartition(Iterator begin, Iterator end) {
  // by default, pick last element as pivot
  Iterator pivot = end - 1;

  // set end to the element just before the pivot, since we don't consider
  // the pivot in the partitioning
  end -= 2;

  while (true) {
    // find the first element greater than the pivot, starting from beginning
    while (*begin < *pivot) {
      ++begin; 
    }

    // find the first element less than the pivot, starting from the end
    while (!(*end < *pivot) && begin < end) {
      --end;
    }

    // if everything is partitioned correctly, then we're done
    if (begin >= end) {
      break;
    }

    // at this point, begin points to an element that is greater than the pivot
    // and on the left side of the pivot. end points to an element that is
    // less than the pivot and on the right side of the pivot. If we swap the
    // two, then the begin and end element will move to the correct side of the
    // pivot.
    std::swap(*begin, *end);
  }

  // move the pivot into the correct location in the array
  std::swap(*begin, *pivot);
  return begin;
}

} // namespace


/**
 * Performs a quicksort.
 *
 * Iterator must be random access.
 *
 * Elements must be < comparable.
 *
 * @param begin begin of range
 * @param end end of range
 */
template<typename Iterator>
void Quicksort(Iterator begin, Iterator end) {

  // done if range is size zero or one
  if (begin >= end || (begin + 1) >= end) {
    return;
  }

  Iterator pivot = QuicksortPartition(begin, end);
  Quicksort(begin, pivot);
  Quicksort(++pivot, end);
}


namespace {

/**
 * Merges two sorted ranges
 *
 * [begin, mid) and [mid, end) should be sorted
 *
 * @param begin begin of the range
 * @param mid middle of the range
 * @param end of the range.
 */
template <typename Iterator>
void Merge(Iterator begin, Iterator mid, Iterator end) {
  using T = typename Iterator::value_type;

  // points to minimum value in the left sorted range
  Iterator left_min = begin;

  // points to minimum value in the right sorted range
  Iterator right_min = mid;

  // we have to merge a total of size elements
  int size = (end - begin);
  std::vector<T> merged(size);

  for (int i = 0; i < size; ++i) {

    // we have elements in both ranges
    if (left_min < mid && right_min < end) {
      
      // add in the smaller of the two minimums
      merged[i] = (*left_min < *right_min) ?
          *left_min++ : *right_min++;

    // right range is empty, so we add in the left minimum
    } else if (left_min < mid) {
      merged[i] = *left_min++;

    // left range is empty, so we add in the right minimum
    } else {
      merged[i] = *right_min++;
    }
  }

  // after merge is done, copy the merged range back into the original range
  std::copy(merged.begin(), merged.end(), begin);
}

} // namespace


/**
 * Performs a mergesort.
 *
 * Iterator must be random access.
 *
 * Elements must be < comparable.
 *
 * @param begin begin of range
 * @param end end of range.
 */
template<typename Iterator>
void Mergesort(Iterator begin, Iterator end) {

  // done if range is size 0 or 1
  if (begin >= end || (begin + 1) >= end) {
    return;
  }
  int mid_offset = (end - begin) / 2;
  Iterator mid = begin + mid_offset;
  Mergesort(begin, mid);
  Mergesort(mid, end);
  Merge(begin, mid, end);
}

} // namespace dsalgo

