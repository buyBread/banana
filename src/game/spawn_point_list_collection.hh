#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every named spawn value; mashed ones get their retail vtable from the type registry
    class spawn_data_base : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x0C];
    };

    struct spawn_point {
        vector3d              position;
        mash::vector
            <spawn_data_base> data;
        bool                  unk_20;
        bool                  unk_21; // spawn picks skip points with it set
        u8                    pad_22[2];
        u32                   unk_24;

        // inlined @ sub_94CB10
        void construct_mashed_class() {
            data.construct_mashed_class();

            unk_24 = 0;
            unk_21 = false;
        }

        // inlined @ sub_93EED0
        void destruct_mashed_class() {
            data.destruct_mashed_class();
        }

        // inlined @ sub_941530
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            data.unmash(mash_info, this, buffer);
        }
    };

    struct spawn_point_list {
        mash::string      name;
        mash::vector
            <spawn_point> points;

        // inlined @ sub_956EA0
        void construct_mashed_class() {
            name  .construct_mashed_class();
            points.construct_mashed_class();
        }

        // inlined @ sub_954000
        void destruct_mashed_class() {
            name  .destruct_mashed_class();
            points.destruct_mashed_class();
        }

        // inlined @ sub_954190
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name  .unmash(mash_info, this, buffer);
            points.unmash(mash_info, this, buffer);
        }
    };

    class spawn_point_list_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::vector
            <spawn_point_list> lists;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (spawn_data_base, 0x10);

    ASSERT_SIZEOF  (spawn_point,         0x28);
    ASSERT_OFFSETOF(spawn_point, data,   0x0C);
    ASSERT_OFFSETOF(spawn_point, unk_20, 0x20);
    ASSERT_OFFSETOF(spawn_point, unk_21, 0x21);
    ASSERT_OFFSETOF(spawn_point, unk_24, 0x24);

    ASSERT_SIZEOF  (spawn_point_list,         0x20);
    ASSERT_OFFSETOF(spawn_point_list, points, 0x0C);

    ASSERT_SIZEOF  (spawn_point_list_collection,        0x18);
    ASSERT_OFFSETOF(spawn_point_list_collection, lists, 0x04);
} // treyarch
