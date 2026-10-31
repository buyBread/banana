#pragma once

#include "game/arch_base.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class trigger;

    namespace references {
        inline util::memory_reference<string_hash> reached_fog { 0x0102C78C };
    } // references

    // a fog marker scripts place; the radar keeps them
    class finger_of_god : public arch_base {

    public:
        vector3d          position;
        i32               poi_index;        // its zoom map poi, -1 for none
        entity_tracker_id tracker_id;       // -1 for none
        arch_base_vhandle ent;              // moves along with set_pos
        trigger*          sphere_trigger;   // its callback raises REACHED_FOG on the fog
        f32               radius;
        u32               unk_28;
        f32               unk_2c;
        bool              poi_active;
        bool              active;
        bool              unk_32;
        bool              unk_33;

        entity_tracker* get_tracker();

        void set_pos(vector3d new_position);
        void set_radius(f32 new_radius); // ignores a negative radius

        void set_poi_active(bool shown);
        void set_active(bool new_active);
        void set_poi_icon(i32 icon, i32 unused);

        void fog_requires_senses(bool requires_senses);
        void arrived_at_fog();
    };

    ASSERT_SIZEOF  (finger_of_god,                 0x34);
    ASSERT_OFFSETOF(finger_of_god, position,       0x08);
    ASSERT_OFFSETOF(finger_of_god, poi_index,      0x14);
    ASSERT_OFFSETOF(finger_of_god, tracker_id,     0x18);
    ASSERT_OFFSETOF(finger_of_god, ent,            0x1C);
    ASSERT_OFFSETOF(finger_of_god, sphere_trigger, 0x20);
    ASSERT_OFFSETOF(finger_of_god, radius,         0x24);
    ASSERT_OFFSETOF(finger_of_god, unk_28,         0x28);
    ASSERT_OFFSETOF(finger_of_god, unk_2c,         0x2C);
    ASSERT_OFFSETOF(finger_of_god, poi_active,     0x30);
    ASSERT_OFFSETOF(finger_of_god, active,         0x31);
    ASSERT_OFFSETOF(finger_of_god, unk_32,         0x32);
    ASSERT_OFFSETOF(finger_of_god, unk_33,         0x33);
} // treyarch
