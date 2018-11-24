#pragma once

#include "Utils.h"
#include <assert.h>
#include <string.h>
#include <new>
#include <algorithm>
#include <iostream>

namespace dsalgo {

/**
 * Double-ended array-based queue. Push back/front and Pop back/front are O(1)
 * amortized. Random insertion/erasure are O(n).
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

  ~Deque() {
    delete[] arr_;
  }

  // TODO copy operator/constructor

  /**
   * Adds the given element to the front of the deque
   *
   * @param e element to add.
   */
  void PushFront(const T& e) {
    if (UNLIKELY(size_ >= underlying_size_)) {
      Resize();
    }

    // if head_idx_ is 0, subtracting 1 would make the insert_idx negative,
    // so we have to add another underlying size
    int insert_idx = (head_idx_ != 0) ?
        head_idx_ - 1 : underlying_size_ - 1;
    new (arr_ + insert_idx) T(e);
    head_idx_ = insert_idx;
    ++size_;
  }

  /**
   * Adds the given element to the end of the deque
   *
   * @param e element to add.
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
    head_idx_ = (head_idx_ < underlying_size_ - 1) ?
      head_idx_ + 1 : 0;
    --size_;
  }

  /**
   * Removes the element at the back of the deque.
   */
  void PopBack() {
    assert(size_ > 0);
    --size_;
  }

  /**
   * Removes the element at the given index. Subsequent elements are shifted
   * towards the front of the deque.
   *
   * @param idx index of element to remove.
   */
  void Erase(int idx) {
    assert(0 <= idx && idx < size_);

    int underlying_idx = GetUnderlyingIdx(idx);
    int end_idx = GetEndIdx();

    // in this case, we only have to shift the wrapped-around part
    if (underlying_idx < head_idx_) {
      std::move(arr_ + underlying_idx + 1, arr_ + end_idx,
          arr_ + underlying_idx);

    // in this case, we have to shift the non-wrapped-around and the
    // wrapped-around part if it exists
    } else {
      if (IsWrappedAround()) {
        std::move(arr_ + underlying_idx + 1, arr_ + underlying_size_,
            arr_ + underlying_idx);
        arr_[underlying_size_ - 1] = std::move(arr_[0]);
        std::move(arr_ + 1, arr_ + GetEndIdx(), arr_ + 0);
      } else {
        std::move(arr_ + underlying_idx + 1, arr_ + head_idx_ + size_,
            arr_ + underlying_idx); 
      }
    }
    --size_;
  }

  /**
   * Inserts the element at the given index. Subsequent elements are shifted
   * towards the back of the deque.
   *
   * @param e the element to insert
   * @param idx index at which to insert the element
   */
  void Insert(const T& e, int idx) {
    assert(0 <= idx && idx <= size_);

    if (size_ < underlying_size_) {
      int underlying_idx = GetUnderlyingIdx(idx);
      int end_idx = GetEndIdx();
      
      // in this case, we only have to shift the wrapped-around part
      if (underlying_idx < head_idx_) {
        std::move_backward(arr_ + underlying_idx, arr_ + end_idx,
            arr_ + end_idx + 1);

      // in this case, we have to shift the non-wrapped-around part and the
      // wrapped-around part if it exists
      } else {

        // if deque has already wrapped-around or if inserting another element
        // would cause it to wrap around, then we need to apply the
        // wrapped-around logic
        if (IsWrappedAround() || head_idx_ + size_ == underlying_size_) {

          // shift wrapped-around part
          std::move_backward(arr_, arr_ + end_idx, arr_ + end_idx + 1); 

          // move last element in underlying array into wrapped-around part
          arr_[0] = std::move(arr_[underlying_size_ - 1]);

          // shift non-wrapped-around part
          std::move_backward(arr_ + underlying_idx, arr_ + underlying_size_ - 1,
              arr_ + underlying_size_);

        // otherwise, inserting would not cause any wrap-around, so the
        // insertion will be just like inserting into a vector
        } else {
          std::move_backward(arr_ + underlying_idx, arr_ + head_idx_ + size_,
              arr_ + head_idx_ + size_ + 1);
        }
      }

      // insert element
      new (arr_ + underlying_idx) T(e);
      ++size_;

    } else {
      if (idx < size_) {
        ResizeAndInsert(e, idx);
      } else {
        PushBack(e);
      }
    }
  }

  /**
   * @return the element at the front of the deque.
   */
  T& Front() const {
    return arr_[head_idx_];
  }

  /**
   * @return the element at the back of the deque.
   */
  T& Back() const {
    return arr_[GetTailIdx()];
  }

  /**
   * @param idx an index
   * @return the element in the deque at index idx
   */
  T& operator[](int idx) const {
    assert(idx < size_);
    return arr_[GetUnderlyingIdx(idx)];
  }

  /**
   * @return the number of elements in the deque.
   */
  int Size() const {
    return size_;
  }

  /**
   * Removes all elements from the deque.
   */
  void Clear() {
    size_ = 0;
    head_idx_ = 0;
  }

private:

  /**
   * Computes the index in the underlying array for the i-th element in the
   * deque. Remember that the deque's head may have shifted if there were
   * PopFronts().
   *
   * @param i an index. Must be between [0, 2*underlying_size_)
   * @return the index of the i-th element in the underlying array.
   */
  inline int GetUnderlyingIdx(int i) const {
    assert(0 <= i && i <= 2 * underlying_size_);
    int summed_idx = head_idx_ + i;
    return summed_idx >= underlying_size_ ?
        summed_idx - underlying_size_ : summed_idx;
  }

  /**
   * @return index of the last element in the deque.
   */
  inline int GetTailIdx() const {
    return GetUnderlyingIdx(size_ - 1);
  }

  /**
   * @return index of the end of the deque (1 spot after the last element in
   * the deque).
   */
  inline int GetEndIdx() const {
    return GetUnderlyingIdx(size_);
  }

  /**
   * @return if this deque has wrapped around past the end of the underlying
   * array.
   */
  inline bool IsWrappedAround() {
    return (head_idx_ + size_) > underlying_size_;
  }

  /**
   * Resizes the underlying array to twice the size and realigns
   * the head with the beginning of the underlying array.
   */
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

  /**
   * Resizes the underlying array to twice the size and inserts the given
   * element while resizing. The head will be realigned with the beginning of
   * the underlying array.
   *
   * @param e the element to insert
   * @param idx the index at which to insert the element. idx must be strictly
   * less than size. If idx == size, the underlying idx becomes 0 and the
   * operation cannot be distinguished from idx == 0. For idx == size, use
   * PushBack instead.
   */
  void ResizeAndInsert(const T& e, int idx) {
    assert(idx < size_);

    T* resized_arr = new T[underlying_size_ * 2];
    int insert_idx = GetUnderlyingIdx(idx);
    if (IsWrappedAround()) {

      // inserting in the wrapped-around part. That means we can copy the
      // non-wrapped-around part over and then adjust copying the wrapped-around
      // part to accommodate the inserted element.
      if (insert_idx < head_idx_) {

        // move [head, end-of-array]
        int next_available_resized_arr_idx = 0;
        std::move(arr_ + head_idx_, arr_ + underlying_size_, resized_arr);
        next_available_resized_arr_idx += (underlying_size_ - head_idx_);

        // move [begin-of-array, insert_idx)
        std::move(arr_, arr_ + insert_idx,
            resized_arr + next_available_resized_arr_idx);
        next_available_resized_arr_idx += insert_idx;

        // insert the element to be inserted
        new (resized_arr + next_available_resized_arr_idx) T(e);
        next_available_resized_arr_idx += 1;

        // move [insert_idx, end-of-array]
        std::move(arr_ + insert_idx, arr_ + GetEndIdx(),
            resized_arr + next_available_resized_arr_idx);

      // inserting in the non-wrapped-around part. That means we need to shift
      // the non-wrapped-around part appropriately to accommodate the inserted
      // element and then we can just copy the wrapped-around part over.
      } else {
        
        // move [head, insert_idx)
        int next_available_resized_arr_idx = 0;
        std::move(arr_ + head_idx_, arr_ + insert_idx, resized_arr);
        next_available_resized_arr_idx += idx;

        // insert the element to be inserted
        new (resized_arr + next_available_resized_arr_idx) T(e);
        next_available_resized_arr_idx += 1;

        // move [insert_idx, end-of-non-wrapped-part)
        std::move(arr_ + insert_idx, arr_ + underlying_size_,
            resized_arr + next_available_resized_arr_idx);
        next_available_resized_arr_idx += (underlying_size_ - insert_idx);

        // move [begin-of-array, end-of-deque]
        std::move(arr_, arr_ + GetEndIdx(),
            resized_arr + next_available_resized_arr_idx);
      }
    } else {
      // resize and insert normally, like it's a vector

      // special case where inserting at the end - we need to move the insert
      // index back to the end
      if (insert_idx == 0) {
        insert_idx = underlying_size_;
      }

      std::move(arr_ + head_idx_, arr_ + insert_idx, resized_arr);
      std::move(arr_ + insert_idx, arr_ + head_idx_ + size_,
          resized_arr + insert_idx + 1);
      new (resized_arr + idx) T(e);
    }
    delete[] arr_;
    arr_ = resized_arr;
    underlying_size_ *= 2;
    ++size_;
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
