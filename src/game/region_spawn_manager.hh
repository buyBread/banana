#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }} // chuck::vm

    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    class igo_3d_text;
    class neighborhood_definition;
    class neighborhood_interface;
    class region_spawn_interface;
    struct neighborhood_interface_unk_04;
    struct region;

    class region_spawn_manager : public singleton<region_spawn_manager, 0x010FA2C4> {

    public:
        u8                             reserved_004[0x04];
        bool                           populate_mission_with_entities;
        u8                             reserved_009[0x03];
        u32                            unk_00c;
        u8                             reserved_010[0x0C];
        region_spawn_interface*        region_spawn_interfaces[8]; // one slot per loaded region
        string_hash                    region_names[8];            // the slot's region name, hashed
        region*                        regions[8];
        u8                             reserved_07c[0x1C];
        neighborhood_interface*        neighborhoods;
        neighborhood_interface_unk_04* unk_09c;                    // the last entry whose condition holds
        neighborhood_definition*       neighborhood;
        neighborhood_definition*       meta_neighborhood;

        static void draw_text();

        void load(const char* level_name, amalga::resource_pack_slot* slot);

        void frame_advance(f32 dt);

        mash::string get_neighborhood_name();
        mash::string get_meta_neighborhood_name();
        mash::string get_city_wide_set_name();

        void update_point_of_interest(chuck::vm::script_instance* inst, mash::string name, vector3d position);
    };

    namespace references {
        inline util::memory_reference<u8> region_spawns_enabled { 0x00BE73FE };

        inline util::memory_reference<igo_3d_text*> region_spawn_text { 0x010F9BC4 };
    } // references

    ASSERT_SIZEOF  (region_spawn_manager,                                 0xA8);
    ASSERT_OFFSETOF(region_spawn_manager, populate_mission_with_entities, 0x08);
    ASSERT_OFFSETOF(region_spawn_manager, unk_00c,                        0x0C);
    ASSERT_OFFSETOF(region_spawn_manager, region_spawn_interfaces,        0x1C);
    ASSERT_OFFSETOF(region_spawn_manager, region_names,                   0x3C);
    ASSERT_OFFSETOF(region_spawn_manager, regions,                        0x5C);
    ASSERT_OFFSETOF(region_spawn_manager, neighborhoods,                  0x98);
    ASSERT_OFFSETOF(region_spawn_manager, unk_09c,                        0x9C);
    ASSERT_OFFSETOF(region_spawn_manager, neighborhood,                   0xA0);
    ASSERT_OFFSETOF(region_spawn_manager, meta_neighborhood,              0xA4);
} // treyarch
