#pragma once

#include "Utils.h"
#include <assert.h>
#include <string.h>
#include <new>
#include <algorithm>
#include <iostream>

namespace dsalgo {

/**
 * Double-ended array-based queue.
 */
template <typename T>
class Deque {

public:

  Deque(int init_size) {
    if (init_size <= 0) {
        init_size = 8;
    }

    init_size = IsPowerOf2(init_size) ?
      init_size :
      NextPowerOf2(init_size);
    arr_ = new T[init_size];
    underlying_size_ = init_size;
  }

  Deque() : Deque(8) {
  }

  /**
   * Adds the given element to the end of the deque
   *
   * @param e Element to add.
   */
  void PushBack(const T& e) {
    if (UNLIKELY(size_ >= underlying_size_)) {
      Resize();
    }
    int insert_idx = GetEndIdx();
    new (arr_ + insert_idx) T(e);
    ++size_;
  }

  /**
   * Removes the element at the front of the deque.
   */
  void PopFront() {
    assert(size_ > 0);
    head_idx_ = (head_idx_ + 1) % underlying_size_;
  }

  /**
   * Gets the element at the front of the deque.
   */
  T& Front() const {
    return arr_[head_idx_];
  }

  T& operator[](int idx) const {
    assert(idx < size_);
    return arr_[GetUnderlyingIdx(idx)];
  }

private:

  /**
   * Computes the index in the underlying array for the i-th element in the
   * deque. Remember that the deque's head may have shifted if there were
   * PopFronts().
   *
   * @return the index of the i-th element in the underlying array.
   */
  int GetUnderlyingIdx(int i) const {
    return (head_idx_ + i) % underlying_size_;
  }

  /**
   * @return index of the last element in the deque.
   */
  int GetTailIdx() const {
    return GetUnderlyingIdx(size_ - 1);
  }

  /**
   * @return index of the end of the deque (1 spot after the last element in
   * the deque).
   */
  int GetEndIdx() const {
    return GetUnderlyingIdx(size_);
  }

  /**
   * @return if this deque has wrapped around past the end of the underlying
   * array.
   */
  bool IsWrappedAround() {
    return (head_idx_ + size_) > underlying_size_;
  }

  void Resize() {
    T* resized_arr = new T[underlying_size_ * 2];

    if (IsWrappedAround()) {

      // move [head, end-of-array] first
      std::move(arr_ + head_idx_, arr_ + underlying_size_, resized_arr);

      // move [begin-of-array, tail] next
      int nonwrapped_size = underlying_size_ - head_idx_;
      std::move(arr_, arr_ + GetEndIdx(), resized_arr + nonwrapped_size);
    } else {

      // if there was no wrap-around, then we're just resizing this like a
      // vector
      std::move(arr_, arr_ + size_, resized_arr);
    }
    delete[] arr_;
    arr_ = resized_arr;
    underlying_size_ *= 2;
    head_idx_ = 0;
  }

private:

  T* arr_ = nullptr;

  /**
   * Total size of underlying array
   */
  int underlying_size_ = 0;


  /**
   * Index of the head of the deque
   */
  int head_idx_ = 0;

  /**
   * Number of elements in the deque.
   * (head_idx_ + size_ - 1) % (underlying_size_) gives you the tail.
   */ 
  int size_ = 0;
};

}
