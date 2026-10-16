#pragma once

#include "treyarch/game/reference.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct district {
        mash::string         name;
        vector3              position;
        reference<ngl::mesh> txtmesh;
        bool                 unk_20; // cleared while the region with this name is loaded
        u8                   unk_21[0x27];

        // inlined @ sub_7DEDF0
        void construct_mashed_class() {
            name   .construct_mashed_class();
            txtmesh.construct_mashed_class();
        }

        // inlined @ sub_7DAAE0
        void destruct_mashed_class() {
            name   .destruct_mashed_class();
            txtmesh.destruct_mashed_class();
        }

        // inlined @ sub_7DCBF0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name   .unmash(mash_info, this, buffer);
            txtmesh.unmash(mash_info, this, buffer);
        }
    };

    class city_lods {

    public:
        void* self; // the resource handler points this at the lods themselves

        mash::vector<district> districts;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (district,           0x48);
    ASSERT_OFFSETOF(district, position, 0x0C);
    ASSERT_OFFSETOF(district, txtmesh,  0x18);
    ASSERT_OFFSETOF(district, unk_20,   0x20);

    ASSERT_SIZEOF  (city_lods,            0x18);
    ASSERT_OFFSETOF(city_lods, districts, 0x04);
} // treyarch
