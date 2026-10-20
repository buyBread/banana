#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct vehicle_model_spec_entry {
        resource_key unk_00;
        u32          unk_08[8];

        // inlined @ sub_596D00
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
        }

        // inlined @ sub_4BA940
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
        }

        // inlined @ sub_594090
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_00.unmash(mash_info, this, buffer);
        }
    };

    class vehicle_model_spec {

    public:
        u32                            unk_00;
        mash::vector
            <vehicle_model_spec_entry> unk_04;
        mash::vector_basic<u32>        unk_18;
        mash::vector_basic<u32>        unk_28;
        u32                            unk_38[3];
        u32                            unk_44;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class vehicle_model {

    public:
        u32                     unk_00;
        u32                     unk_04[2];
        u32                     unk_0c;
        u32                     unk_10[2];
        mash::vector_basic<u32> extra_member_ids;
        resource_key            unk_28;
        u32                     unk_30[2];
        void*                   unk_38; // set by construct

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class vehicle_model_group {

    public:
        u32                     unk_00;
        mash::vector_basic<u32> unk_04;
        mash::vector_basic<u32> unk_14;
        u32                     unk_24;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the level's type-28 "vehicles" resource
    class vehicle_model_depot {

    public:
        void* self; // the resource handler points this at the depot itself

        mash::vector
            <vehicle_model_spec>  specs;
        mash::vector
            <vehicle_model>       models;
        mash::vector
            <vehicle_model_group> groups;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (vehicle_model_spec_entry, 0x28);

    ASSERT_SIZEOF  (vehicle_model_spec,         0x48);
    ASSERT_OFFSETOF(vehicle_model_spec, unk_04, 0x04);
    ASSERT_OFFSETOF(vehicle_model_spec, unk_18, 0x18);
    ASSERT_OFFSETOF(vehicle_model_spec, unk_28, 0x28);
    ASSERT_OFFSETOF(vehicle_model_spec, unk_44, 0x44);

    ASSERT_SIZEOF  (vehicle_model,                   0x3C);
    ASSERT_OFFSETOF(vehicle_model, unk_0c,           0x0C);
    ASSERT_OFFSETOF(vehicle_model, extra_member_ids, 0x18);
    ASSERT_OFFSETOF(vehicle_model, unk_28,           0x28);
    ASSERT_OFFSETOF(vehicle_model, unk_38,           0x38);

    ASSERT_SIZEOF  (vehicle_model_group,         0x28);
    ASSERT_OFFSETOF(vehicle_model_group, unk_04, 0x04);
    ASSERT_OFFSETOF(vehicle_model_group, unk_14, 0x14);
    ASSERT_OFFSETOF(vehicle_model_group, unk_24, 0x24);

    ASSERT_SIZEOF  (vehicle_model_depot,         0x40);
    ASSERT_OFFSETOF(vehicle_model_depot, specs,  0x04);
    ASSERT_OFFSETOF(vehicle_model_depot, models, 0x18);
    ASSERT_OFFSETOF(vehicle_model_depot, groups, 0x2C);

    namespace references {
        // set by game::load_this_level; cleared by game::unload_current_level
        inline util::memory_reference<vehicle_model_depot*> vehicle_model_depot { 0x00FC7A90 };
    } // references
} // treyarch
