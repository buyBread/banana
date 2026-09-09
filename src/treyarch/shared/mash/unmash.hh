#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    template<typename T>
    T* unmash_in_place(void* image,
                       i32   image_size,
                       bool  has_header = true) {

        mash_info_struct mash_info(UNMASH_MODE, (u8*)image, image_size, has_header);

        T* result = nullptr;

        // generated roots aren't special: consume one class, walk its members, then run the ownership pass
        mash_info.unmash_class(result, nullptr, NORMAL_BUFFER);
        mash_info_struct::construct_class(result);

        return result;
    }
}} // treyarch::mash
