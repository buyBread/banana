#pragma once

#include "treyarch/game/reference.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class tilemap {

    public:
        void*                                self; // the resource handler points this at the tilemap itself
        reference<ngl::texture>              unk_04[4];
        reference<meshmap>                   unk_34;
        u32                                  unk_40[3];
        reference<ngl::texture>              unk_4c[4];
        reference<adjustment_layers_library> unk_7c;
        u32                                  unk_88[3];
        reference<ngl::texture>              unk_94[2];
        u32                                  unk_ac;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (tilemap,         0xB0);
    ASSERT_OFFSETOF(tilemap, unk_34, 0x34);
    ASSERT_OFFSETOF(tilemap, unk_4c, 0x4C);
    ASSERT_OFFSETOF(tilemap, unk_7c, 0x7C);
    ASSERT_OFFSETOF(tilemap, unk_94, 0x94);
    ASSERT_OFFSETOF(tilemap, unk_ac, 0xAC);
} // treyarch
