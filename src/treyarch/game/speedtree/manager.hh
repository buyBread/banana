#pragma once

#include "treyarch/game/speedtree/wrapper.hh"
#include "treyarch/game/wds/subdivision_visitor.hh"
#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct speed_tree_manager_entry {
        speed_tree_wrapper* wrappers[2];
        u32                 unk_08;
    };

    // released instances wait one update before their trees are torn down and they're reused
    struct speed_tree_instance_pool {
        u32                  unk_00;
        speed_tree_instance* free_instances;
        speed_tree_instance* releasing_instances;
        speed_tree_instance* pending_instances;

        void recycle();
    };

    // SpeedTreeManager::visitor_t
    struct speed_tree_manager_visitor {
        subdivision_visitor base;
        vector3d            position;
    };

    class speed_tree_manager {

    public:
        u8                       reserved_000[0x17C];
        speed_tree_manager_entry entries[6];
        u8                       reserved_1c4[0x08];
        i32                      entry_count;
        speed_tree_instance_pool instance_pool;

        static void update_camera_and_wind(const matrix4x4* camera);

        void update();
        void render(bool billboards_only);
    };

    namespace references {
        inline util::memory_reference<speed_tree_manager> speed_tree_manager { 0x01087188 };

        inline util::memory_reference<u32> speed_trees_enabled { 0x00F4CD48 };

        inline util::memory_reference<void*> speed_tree_manager_visitor_vtable { 0x00BD9728 };

        // wind object inputs; nothing else reads or writes them
        inline util::memory_reference<f32>     speed_tree_wind_unk_01086f18 { 0x01086F18 };
        inline util::memory_reference<vector3d> speed_tree_wind_unk_00e80cf0 { 0x00E80CF0 };

        inline util::memory_reference<f32> speed_tree_wind_start_time       { 0x01087F24 };
        inline util::memory_reference<u32> speed_tree_wind_start_time_guard { 0x01087F28 };
    } // references

    ASSERT_SIZEOF  (speed_tree_manager_entry, 0x0C);

    ASSERT_SIZEOF  (speed_tree_instance_pool,                      0x10);
    ASSERT_OFFSETOF(speed_tree_instance_pool, free_instances,      0x04);
    ASSERT_OFFSETOF(speed_tree_instance_pool, releasing_instances, 0x08);
    ASSERT_OFFSETOF(speed_tree_instance_pool, pending_instances,   0x0C);

    ASSERT_SIZEOF  (speed_tree_manager_visitor,           0x14);
    ASSERT_OFFSETOF(speed_tree_manager_visitor, position, 0x08);

    ASSERT_OFFSETOF(speed_tree_manager, entries,       0x17C);
    ASSERT_OFFSETOF(speed_tree_manager, entry_count,   0x1CC);
    ASSERT_OFFSETOF(speed_tree_manager, instance_pool, 0x1D0);
} // treyarch
