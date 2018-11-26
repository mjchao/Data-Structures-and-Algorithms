#pragma once

#include <utility>
#include <iostream>

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
 * @param begin begin of range
 * @param end end of range
 */
template<typename Iterator>
void Quicksort(Iterator begin, Iterator end) {

  // done if range is size zero
  if ((begin + 1) >= end) {
    return;
  }

  Iterator pivot = QuicksortPartition(begin, end);
  Quicksort(begin, pivot);
  Quicksort(++pivot, end);
}


/**
 * Performs a mergesort.
 */
template<typename Iterator>
void Mergesort(Iterator begin, Iterator end) {

}

} // namespace dsalgo

