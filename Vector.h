#pragma once

#include "Utils.h"


namespace dsalgo {

/**
 * An array-based list.
 */ 
template <typename T>
class Vector {

public:

  Vector(int size) {
    int arr_size = NextPowerOf2(size);
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

