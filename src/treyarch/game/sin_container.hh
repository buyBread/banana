#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // copied onto the district graph's region of the same name by sin_container::setup_world
    struct sin_district_info {
        u32 unk_00;
        f32 unk_04; // left alone when FLT_MAX
        u32 flags;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct sin_district_container {
        mash::string      district_name;
        sin_district_info info;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the level's type-2 resource, named after the level
    struct sin_container {
        void*                        self; // the resource handler points this at the container itself
        mash::string                 master_script_name;
        mash::string                 district_graph_name;
        u32                          unk_1C;
        mash::vector
            <sin_district_container> districts;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sin_district_info,         0x0C);
    ASSERT_OFFSETOF(sin_district_info, unk_04, 0x04);
    ASSERT_OFFSETOF(sin_district_info, flags,  0x08);

    ASSERT_SIZEOF  (sin_district_container,       0x18);
    ASSERT_OFFSETOF(sin_district_container, info, 0x0C);

    ASSERT_SIZEOF  (sin_container,                      0x34);
    ASSERT_OFFSETOF(sin_container, master_script_name,  0x04);
    ASSERT_OFFSETOF(sin_container, district_graph_name, 0x10);
    ASSERT_OFFSETOF(sin_container, unk_1C,              0x1C);
    ASSERT_OFFSETOF(sin_container, districts,           0x20);
} // treyarch
