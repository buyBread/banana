#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct dsg_region_container {
        mash::string            name;
        u8                      unk_0C[0x44];
        mash::vector_basic<i32> unk_50;
        u32                     unk_60;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct dsg_box_container {
        u32 unk_00[13];

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // the level's type-6 .dsg resource, named by its sin_container
    class district_graph_container {

    public:
        void* self; // the resource handler points this at the graph itself
        
        mash::vector
            <dsg_region_container> regions;
        mash::vector
            <dsg_box_container>    boxes;
        u32                        unk_2C;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (dsg_region_container,         0x64);
    ASSERT_OFFSETOF(dsg_region_container, unk_0C, 0x0C);
    ASSERT_OFFSETOF(dsg_region_container, unk_50, 0x50);
    ASSERT_OFFSETOF(dsg_region_container, unk_60, 0x60);

    ASSERT_SIZEOF(dsg_box_container, 0x34);

    ASSERT_SIZEOF  (district_graph_container,          0x30);
    ASSERT_OFFSETOF(district_graph_container, regions, 0x04);
    ASSERT_OFFSETOF(district_graph_container, boxes,   0x18);
    ASSERT_OFFSETOF(district_graph_container, unk_2C,  0x2C);
} // treyarch
