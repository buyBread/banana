#pragma once

#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class arch_base;

    // the axes are scaled by their half extents
    class box_trigger_info {

    public:
        void* self; // the resource handler points this at the info itself

        mash::string ent_id;
        vector3d     x_axis;
        vector3d     y_axis;
        vector3d     z_axis;
        vector3d     pos;
        arch_base*   trig; // the box_trigger the handler creates

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (box_trigger_info,         0x44);
    ASSERT_OFFSETOF(box_trigger_info, ent_id, 0x04);
    ASSERT_OFFSETOF(box_trigger_info, x_axis, 0x10);
    ASSERT_OFFSETOF(box_trigger_info, pos,    0x34);
    ASSERT_OFFSETOF(box_trigger_info, trig,   0x40);
} // treyarch
