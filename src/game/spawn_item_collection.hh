#pragma once

#include <cstddef>

#include "game/math/algebra.hh"
#include "game/progression_change.hh"
#include "game/quest_descriptions.hh"
#include "game/spawn_table_interface.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every no-spawn area; mashed ones get their retail vtable from the type registry, the spawn table manager builds its own
    class no_spawn_base : public mash::mash_virtual_base {

    public:
        mash::string name;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        // sub_71FF10
        virtual bool contains_point(const vector3d*) { return false; }
    };

    class no_spawn_point : public no_spawn_base {

    public:
        vector3d center;
        f32      radius;

        // sub_905AB0
        no_spawn_point() {} // the centre and radius stay as allocated

        // sub_90F940
        bool contains_point(const vector3d* point) override {
            f32 dx = (f32)((f64)point->x - (f64)center.x);
            f32 dy = (f32)((f64)point->y - (f64)center.y);
            f32 dz = (f32)((f64)point->z - (f64)center.z);

            f32 distance_squared = (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);
            f32 radius_squared   = (f32)((f64)radius * radius);

            return radius_squared > distance_squared;
        }
    };

    // a polygon over x and z
    class no_spawn_quad : public no_spawn_base {

    public:
        mash::vector_basic<vector3d> points;

        // sub_929930
        no_spawn_quad() {
            points.array    = nullptr;
            points.max_size = 0;
        }

        // sub_920B40
        bool contains_point(const vector3d* point) override { // even-odd crossings of the edges
            f32 x = point->x;
            f32 z = point->z;

            bool inside   = false;
            u32  previous = points.size - 1;

            for (u32 index = 0; index < points.size; ++index) {
                const vector3d &a = points.array[index];
                const vector3d &b = points.array[previous];

                if ((z > a.z && b.z >= z) ||
                    (z > b.z && a.z >= z)) {

                    f64 crossing = ((f64)z - (f64)a.z) / ((f64)b.z - (f64)a.z) * ((f64)b.x - (f64)a.x) + (f64)a.x;

                    if (x > crossing)
                        inside = !inside;
                }

                previous = index;
            }

            return inside;
        }
    };

    // the spawn table manager keeps the last one whose condition holds
    struct spawn_item_collection_unk_38 {
        mash::string     name;
        mash::string     expression;
        u32              low_spawn_count;
        u32              high_spawn_count;
        f32              unk_20;
        f32              ramp_seconds;    // to climb from the low count to the high one
        f32              give_up_seconds; // stepping down before climbing again
        f32              hold_seconds;    // at the low count
        u32              step_count;      // steps down from the high count
        quest_condition* condition;

        spawn_item_collection_unk_38();
        ~spawn_item_collection_unk_38();

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the spawn table manager keeps the last one whose condition holds
    struct spawn_item_collection_unk_4c {
        mash::string        name;
        mash::string        expression;
        mash::vector
            <no_spawn_base> no_spawn_areas;
        quest_condition*    condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class spawn_item_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::string                       name;
        mash::vector
            <progression_change>           changes;
        mash::vector
            <spawn_table_unk_68>           unk_24;
        mash::vector
            <spawn_item_collection_unk_38> pacings;
        mash::vector
            <spawn_item_collection_unk_4c> no_spawn_zone_sets;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (no_spawn_base,       0x10);
    ASSERT_OFFSETOF(no_spawn_base, name, 0x04);

    ASSERT_SIZEOF  (no_spawn_point,         0x20);
    ASSERT_OFFSETOF(no_spawn_point, center, 0x10);
    ASSERT_OFFSETOF(no_spawn_point, radius, 0x1C);

    ASSERT_SIZEOF  (no_spawn_quad,         0x20);
    ASSERT_OFFSETOF(no_spawn_quad, points, 0x10);

    ASSERT_SIZEOF  (spawn_item_collection_unk_38,                   0x38);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, low_spawn_count,  0x18);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, high_spawn_count, 0x1C);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, ramp_seconds,     0x24);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, give_up_seconds,  0x28);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, hold_seconds,     0x2C);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, step_count,       0x30);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, condition,        0x34);

    ASSERT_SIZEOF  (spawn_item_collection_unk_4c,                 0x30);
    ASSERT_OFFSETOF(spawn_item_collection_unk_4c, no_spawn_areas, 0x18);
    ASSERT_OFFSETOF(spawn_item_collection_unk_4c, condition,      0x2C);

    ASSERT_SIZEOF  (spawn_item_collection,                     0x60);
    ASSERT_OFFSETOF(spawn_item_collection, name,               0x04);
    ASSERT_OFFSETOF(spawn_item_collection, changes,            0x10);
    ASSERT_OFFSETOF(spawn_item_collection, unk_24,             0x24);
    ASSERT_OFFSETOF(spawn_item_collection, pacings,            0x38);
    ASSERT_OFFSETOF(spawn_item_collection, no_spawn_zone_sets, 0x4C);
} // treyarch
