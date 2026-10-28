#pragma once

#include "game/arch_base.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/singleton.hh"
#include "treyarch/mash/string_hash.hh"
#include "vc8/list.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    enum e_trigger_flags : u32 {
        trigger_flag_active                 = 0x00000001,
        trigger_flag_use_any_char           = 0x00000002,
        trigger_flag_multiple_entrance      = 0x00000004,
        trigger_flag_sees_dead_people       = 0x00000008,
        trigger_flag_occupied               = 0x00000010,
        trigger_flag_extended_events        = 0x00000020, // enter and leave also raise an event naming the occupant
        trigger_flag_owned_by_conglom       = 0x00000080,
        trigger_flag_owned_by_entity_base   = 0x00000100,
        trigger_flag_tied_to_attach_root    = 0x00000200,
        trigger_flag_tied                   = 0x00000400,
        trigger_flag_pending_refresh        = 0x00000800,
        trigger_flag_code_owned             = 0x00001000, // a code system's own; scripts leave it be
        trigger_flag_unk_2000               = 0x00002000,
        trigger_flag_unk_4000               = 0x00004000,
        trigger_flag_in_all_broadphase      = 0x40000000,
        trigger_flag_in_any_char_broadphase = 0x80000000
    };

    // SM3's trigger; box_trigger and sphere_trigger give it a shape
    class trigger : public arch_base {

    public:
        trigger*                       occupied_prev;  // trigger_manager's occupied list
        trigger*                       occupied_next;
        trigger*                       all_prev;
        trigger*                       all_next;
        trigger*                       refresh_prev;
        trigger*                       refresh_next;
        string_hash                    id;
        u32                            flags;
        trigger*                       next_tied;      // the next one tied to the same entity
        arch_base_vhandle              tied_entity;
        vc8::list<arch_base_vhandle>*  contained_list; // who's inside, kept while occupied
        arch_base_vhandle              occupant;       // a single-entrance trigger's

        // slot 54
        bool point_inside(const vector3d* point) {
            return ((bool (__thiscall*)(trigger*, const vector3d*))vtable[54])(this, point);
        }

        // slot 56
        bool is_box_trigger() const {
            return ((bool (__thiscall*)(const trigger*))vtable[56])(this);
        }

        // slot 57
        bool is_sphere_trigger() const {
            return ((bool (__thiscall*)(const trigger*))vtable[57])(this);
        }

        // slot 59
        f32 get_radius() const {
            return ((f32 (__thiscall*)(const trigger*))vtable[59])(this);
        }

        // slot 60
        vector3d get_abs_position() const {
            vector3d result;

            ((vector3d* (__thiscall*)(const trigger*, vector3d*))vtable[60])(this, &result);

            return result;
        }

        // slot 61
        void set_offset(const vector3d* offset) {
            ((void (__thiscall*)(trigger*, const vector3d*))vtable[61])(this, offset);
        }

        bool is_code_owned() const {
            return flags & trigger_flag_code_owned;
        }

        void set_uses_extended_events(bool on);
    };

    class trigger_manager : public singleton_instance<trigger_manager, 0x0102FE48> {

    public:
        u32 unk_0000[2];        // the 8-byte type sub_95C220 zeroes
        u8  unk_0008;
        u8  unk_0009;
        u8  pad_000a[2];
        u32 unk_000c[2];        // same type
        u32 unk_0014[11];
        u32 unk_0040[1024][2];
        u32 unk_2040;

        trigger_manager();

        static void create_inst();
    };

    ASSERT_SIZEOF  (trigger,                 0x38);
    ASSERT_OFFSETOF(trigger, id,             0x20);
    ASSERT_OFFSETOF(trigger, flags,          0x24);
    ASSERT_OFFSETOF(trigger, contained_list, 0x30);
    ASSERT_OFFSETOF(trigger, occupant,       0x34);

    ASSERT_SIZEOF  (trigger_manager,           0x2044);
    ASSERT_OFFSETOF(trigger_manager, unk_000c, 0x000C);
    ASSERT_OFFSETOF(trigger_manager, unk_0040, 0x0040);
    ASSERT_OFFSETOF(trigger_manager, unk_2040, 0x2040);
} // treyarch
