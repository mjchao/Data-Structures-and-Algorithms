#pragma once

#include "Utils.h"


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
    
    int arr_size = IsPowerOf2(init_size) ?
      init_size :
      NextPowerOf2(init_size);
    arr_ = new T[arr_size];
  }

  Vector() : Vector(8) {
  }

  ~Vector() {
    delete[] arr_;
  }


private:

  T* arr_;
};

} // namespace dsalgo

