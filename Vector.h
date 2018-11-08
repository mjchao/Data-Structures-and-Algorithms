#pragma once

#include "Utils.h"
#include <string.h>


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

private:
  
  /**
   * Resizes the underlying array by doubling its size.
   */
  void Resize() {
    T* resized_arr = new T[underlying_size_ * 2];
    memcpy(resized_arr, arr_, underlying_size_);
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

