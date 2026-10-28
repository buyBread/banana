#pragma once

#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mash {
    template<typename T>
    struct vector_basic_ref_count_internals {
        vector_basic<T>* data;
        u16              ref_count;
        bool             dynamic;
        u8               pad_07;
    };

    // several owners share one vector through the internals
    template<typename T>
    class vector_basic_ref_counted {

    public:
        vector_basic_ref_count_internals<T>* internals;

        u32 size() const {
            return internals->data->size;
        }

        bool empty() const {
            return internals->data->empty();
        }

              T &operator[](u16 index)       { return internals->data->data[index]; }
        const T &operator[](u16 index) const { return internals->data->data[index]; }

              vector_basic<T> &get_vector()       { return *internals->data; }
        const vector_basic<T> &get_vector() const { return *internals->data; }
    };

    ASSERT_SIZEOF  (vector_basic_ref_count_internals<u32>,            0x08);
    ASSERT_OFFSETOF(vector_basic_ref_count_internals<u32>, ref_count, 0x04);
    ASSERT_SIZEOF  (vector_basic_ref_counted<u32>,                    0x04);
}} // treyarch::mash
