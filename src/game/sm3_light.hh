#pragma once

#include "game/reference.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/math/matrix.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class sm3_light {

    public:
        void*                     self; // the resource handler points this at the light itself
        mash::string              name;
        u32                       unk_10[16];
        u32                       unk_50[3];
        u32                       unk_5c[3];
        u32                       unk_68[3];
        math::mat43::packed       po;
        reference<light_behavior> behavior;
        reference<ngl::texture>   gobo;
        u32                       unk_b8;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sm3_light,           0xBC);
    ASSERT_OFFSETOF(sm3_light, name,     0x04);
    ASSERT_OFFSETOF(sm3_light, unk_50,   0x50);
    ASSERT_OFFSETOF(sm3_light, po,       0x74);
    ASSERT_OFFSETOF(sm3_light, behavior, 0xA4);
    ASSERT_OFFSETOF(sm3_light, gobo,     0xAC);
    ASSERT_OFFSETOF(sm3_light, unk_b8,   0xB8);
} // treyarch
