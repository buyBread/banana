#pragma once

#include "treyarch/mash/string.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }} // chuck::vm

    class igo_3d_text;

    class region_spawn_manager : public singleton<region_spawn_manager, 0x010FA2C4> {

    public:
        u8   reserved_004[0x04];
        bool populate_mission_with_entities;

        static void draw_text();

        void update_point_of_interest(chuck::vm::script_instance* inst, mash::string name, vector3d position);
    };

    namespace references {
        inline util::memory_reference<u8> region_spawns_enabled { 0x00BE73FE };

        inline util::memory_reference<igo_3d_text*> region_spawn_text { 0x010F9BC4 };
    } // references

    ASSERT_OFFSETOF(region_spawn_manager, populate_mission_with_entities, 0x08);
} // treyarch
