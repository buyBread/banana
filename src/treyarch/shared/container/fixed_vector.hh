#pragma once

#include "util/types.hh"

namespace treyarch { namespace container {
    template<typename T, u32 N>
    struct fixed_vector { // L85462: SM3 .ii
        T   data_[N];
        u32 size_;

        fixed_vector() : size_(0) {}

        T &push_back(const T &value) {
            return data_[size_++] = value;
        }

              T* begin()       { return data_; }
        const T* begin() const { return data_; }
              T* end()         { return data_ + size_; }
        const T* end()   const { return data_ + size_; }

        u32  size()  const { return size_; }
        bool empty() const { return size_ == 0; }
    };
}} // treyarch::container
