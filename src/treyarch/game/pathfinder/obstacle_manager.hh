#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace pathfinder {
    // RTTI pathfinder::obstacle_manager; not navmesh_obstacle_manager
    class obstacle_manager {

    public:
        void* vtable;
        u32   unk_04;

        obstacle_manager();

        static void create_inst();
    };

    namespace references {
        inline util::memory_reference<obstacle_manager*> obstacle_manager { 0x00FC3468 };

        inline util::memory_reference<void*> obstacle_manager_vtable { 0x00B89100 };
    } // references

    ASSERT_SIZEOF(obstacle_manager, 0x08);
}} // treyarch::pathfinder
