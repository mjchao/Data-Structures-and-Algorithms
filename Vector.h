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
    memcpy(arr_, other.arr_, other.size_ * sizeof(T));
  }

  Vector<T>& operator=(const Vector<T>& other) {
    delete[] arr_;
    size_ = other.size_;
    underlying_size_ = other.underlying_size_;
    arr_ = new T[underlying_size_];
    memcpy(arr_, other.arr_, other.size_ * sizeof(T));
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
    int elements_to_shift = (size_ - (idx + 1));
    int bytes_to_shift = elements_to_shift * sizeof(T);

    // for fewer bytes, faster to avoid manually move than to use memmove.
    if (bytes_to_shift <= 64) {
      for (int i = idx; i < size_ - 1; ++i) {
        arr_[i] = std::move(arr_[i + 1]);
      }

    // for larger shifts, we'll just use memmove.
    } else {
      memmove(arr_ + idx, arr_ + idx + 1, bytes_to_shift);
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
		if (UNLIKELY(size_ >= underlying_size_)) {
		  Resize();		
		}

    int elements_to_shift = (size_ - idx);
    int bytes_to_shift = elements_to_shift * sizeof(T);

    if (bytes_to_shift <= 64) {
      for (int i = size_ - 1; i >= idx; --i) {
        arr_[i + 1] = std::move(arr_[i]);
      }
    } else {
      memmove(arr_ + idx + 1, arr_ + idx, bytes_to_shift);
    }

    new (arr_ + idx) T(e);
    ++size_;
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
    memcpy(resized_arr, arr_, underlying_size_ * sizeof(T));
    delete[] arr_;
    arr_ = resized_arr;
    underlying_size_ *= 2;
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

