#pragma once

#include "banana/util/types.hh"

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

              T &back()       { return data_[size_ - 1]; }
        const T &back() const { return data_[size_ - 1]; }

              T &operator[](u32 index)       { return data_[index]; }
        const T &operator[](u32 index) const { return data_[index]; }

        void clear() {
            size_ = 0;
        }

        // keeps the order
        void erase(u32 index) {
            --size_;

            for (u32 position = index; position < size_; ++position)
                data_[position] = data_[position + 1];
        }

        u32  size()  const { return size_; }
        bool empty() const { return size_ == 0; }
    };
}} // treyarch::container
