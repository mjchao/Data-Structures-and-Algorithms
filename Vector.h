#pragma once

#include "Utils.h"
#include <string.h>
#include <assert.h>
#include <new>
#include <iostream>

namespace dsalgo {

/**
 * An array-based list.
 */ 
template <typename T>
class Vector {

public:

  Vector(int init_size) {
    if (init_size <= 0) {
      init_size = 8;
    }
    
    init_size = IsPowerOf2(init_size) ?
      init_size :
      NextPowerOf2(init_size);
    arr_ = new T[init_size];
    underlying_size_ = init_size;
  }

  Vector() : Vector(8) {
  }

  Vector(const Vector<T>& other) {
    size_ = other.size_;
    underlying_size_ = other.underlying_size_;
    arr_ = new T[underlying_size_];
    for (int i = 0; i < other.size_; ++i) {
      new (arr_ + i) T(other.arr_[i]);
    }
  }

  Vector<T>& operator=(const Vector<T>& other) {
    delete[] arr_;
    size_ = other.size_;
    underlying_size_ = other.underlying_size_;
    arr_ = new T[underlying_size_];
    for (int i = 0; i < other.size_; ++i) {
      new (arr_ + i) T(other.arr_[i]);
    }
    return *this;
  }

  T& operator[](int idx) {
    assert(idx < size_);
    return arr_[idx];
  }

  ~Vector() {
    delete[] arr_;
  }

  void PushBack(const T& e) {
    if (size_ < underlying_size_) {
      new (arr_ + size_) T(e);
    } else {
      Resize(); 
      new (arr_ + size_) T(e);
    }
    ++size_;
  }

  T PopBack() {
    assert(size_ > 0);
    T rtn = std::move(arr_[size_ - 1]);
    --size_;
    return std::move(rtn);
  }

  T PopFront() {
    return std::move(Erase(0));
  }

  /**
   * Removes the element at index idx. Subsequent elements are shifted down.
   *
   * @param idx Index of the element to remove.
   * @return The element that was removed.
   */
  T Erase(int idx) {
    assert(idx < size_);
    T rtn = std::move(arr_[idx]);

    int num_elems_to_shift = (size_ - (idx + 1));

    // if only shifting a few elements, then we can shift them one-by-one
    // it'll be faster than std::move on a range.
    if (num_elems_to_shift <= 32) {
      for (int i = idx; i < size_ - 1; ++i) {
        arr_[i] = std::move(arr_[i + 1]);
      }

    // if shifting a lot of elements, then we can parallelize with std::move
    // on a range and be faster than shifting one at a time.
    } else {
      std::move(arr_ + idx + 1, arr_ + size_, arr_ + idx);
    }
    --size_;
    return std::move(rtn);
  }

  /**
   * Inserts the element at the given index. Subsequent elements are shifted
   * up.
   *
   * @param e Element to insert
   * @param idx Index at which to insert the element.
   */
  void Insert(const T& e, int idx) {
    assert(idx <= size_);

    if (LIKELY(size_ < underlying_size_)) {
      std::move_backward(arr_ + idx, arr_ + size_, arr_ + size_ + 1);
      new (arr_ + idx) T(e);
      ++size_;
    } else {
      ResizeAndInsert(e, idx);
		}
  }

  int Size() {
    return size_;
  }

  void Clear() {
    size_ = 0;
  }

private:
  
  /**
   * Resizes the underlying array by doubling its size.
   */
  void Resize() {
    T* resized_arr = new T[underlying_size_ * 2];
    std::move(arr_, arr_ + size_, resized_arr);
    delete[] arr_;
    arr_ = resized_arr;
    underlying_size_ *= 2;
  }

  void ResizeAndInsert(const T& e, int idx) {
    T* resized_arr = new T[underlying_size_ * 2];
    std::move(arr_, arr_ + idx, resized_arr);
    new (resized_arr + idx) T(e);
    std::move(arr_ + idx, arr_ + size_, resized_arr + idx + 1);
    delete[] arr_;
    arr_ = resized_arr;
    underlying_size_ *= 2;
    ++size_;
  }


private:

  T* arr_ = nullptr;

  /**
   * Size of underlying array
   */
  int underlying_size_ = 0;

  /**
   * Number of elements in this vector.
   */
  int size_ = 0;

};

} // namespace dsalgo

