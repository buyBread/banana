#pragma once

#include "treyarch/shared/mash/container_base.hh"
#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace mash {
    template <class T>
    class vector_basic : public container_base {

    public:
        static const i32 block_alloc_size = 8;

    public:
        T*  data;
        u32 capacity;
    };

    ASSERT_SIZEOF  (vector_basic<void*>,                     0x10);
    ASSERT_OFFSETOF(vector_basic<void*>, mash_image_offset,  0x00);
    ASSERT_OFFSETOF(vector_basic<void*>, size,               0x04);
    ASSERT_OFFSETOF(vector_basic<void*>, data,               0x08);
    ASSERT_OFFSETOF(vector_basic<void*>, capacity,           0x0C);
}} // treyarch::mash
