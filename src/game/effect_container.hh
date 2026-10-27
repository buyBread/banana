#pragma once

#include "game/sm3_light.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class effect_container {

    public:
        enum e_space : i32 {
            space_weird,
            space_world,
            space_local
        };

        void*                   self; // the resource handler points this at the container itself
        mash::vector<sm3_light> light;
        e_space                 space;
        u32                     unk_1c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (effect_container,         0x20);
    ASSERT_OFFSETOF(effect_container, light,  0x04);
    ASSERT_OFFSETOF(effect_container, space,  0x18);
    ASSERT_OFFSETOF(effect_container, unk_1c, 0x1C);
} // treyarch
