#pragma once

#include "game/reference.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class phat_palette {

    public:
        void* self; // the resource handler points this at the palette itself

        reference<ngl::texture> palettized_output;
        reference<ngl::texture> palette;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (phat_palette,                    0x1C);
    ASSERT_OFFSETOF(phat_palette, palettized_output, 0x04);
    ASSERT_OFFSETOF(phat_palette, palette,           0x10);
} // treyarch
