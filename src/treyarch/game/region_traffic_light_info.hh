#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct region_traffic_light {
        u32 unk_00;
        u32 unk_04;
        i32 unk_08;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // each region's type-13 resource, named after the region
    class region_traffic_light_info {

    public:
        void* self; // the resource handler points this at the info itself
        
        mash::vector
            <region_traffic_light> lights;

        // sub_7A5360
        void construct_mashed_class() {
            self = nullptr;

            lights.construct_mashed_class();
        }

        // sub_7A30F0
        void destruct_mashed_class() {
            lights.destruct_mashed_class();
        }

        // sub_7A6E90
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            lights.unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF  (region_traffic_light, 0x0C);

    ASSERT_SIZEOF  (region_traffic_light_info,         0x18);
    ASSERT_OFFSETOF(region_traffic_light_info, lights, 0x04);
} // treyarch
