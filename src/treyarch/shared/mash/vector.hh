#pragma once

#include "util/types.hh"

namespace treyarch { namespace mash {
    template<typename T>
    struct vector {
        u32  mash_image_extent; // bounds the vector’s embedded mash image
        u32  size;
        T**  data;
        u32  capacity;
        bool destroy_elements;
        u8   pad[3];
    };
}} // treyarch::mash