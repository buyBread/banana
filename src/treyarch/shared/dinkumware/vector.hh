#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

/*
    todo: this will need to be more properly implemented
          when? idk, we'll see
*/

namespace treyarch { namespace dinkumware {
    template<typename T>
    class vector_allocator {};

    template<typename T, typename allocator_t = vector_allocator<T>>
    class vector {

        allocator_t allocator;

        T* first;
        T* last;
        T* capacity_end;

public:
              T &operator[](u32 index)       noexcept { return first[index]; }
        const T &operator[](u32 index) const noexcept { return first[index]; }

              T* begin()       noexcept { return first; }
        const T* begin() const noexcept { return first; }

              T* end()       noexcept { return last; }
        const T* end() const noexcept { return last; }

              T &back()       noexcept { return last[-1]; }
        const T &back() const noexcept { return last[-1]; }

        u32 size() const noexcept {
            return first ? (u32)(last - first) : 0;
        }

        u32 capacity() const noexcept {
            return first ? (u32)(capacity_end - first) : 0;
        }

        bool empty() const noexcept { return first == last; }
    };

    ASSERT_SIZEOF(vector<void*>, 0x10);
}} // treyarch::dinkumware
