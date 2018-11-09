#pragma once

#include "Utils.h"
#include <string.h>
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
    return std::move(Erase(size_ - 1));
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
    T rtn = std::move(arr_[idx]);
    int elements_to_shift = (size_ - (idx + 1));
    memmove(arr_ + idx, arr_ + idx + 1, elements_to_shift * sizeof(T));
    --size_;
    return std::move(rtn);
  }

  int Size() {
    return size_;
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

