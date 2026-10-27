#pragma once

#include "game/wds/entity/entity.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }} // chuck::vm

    namespace nal {
        struct skeleton_map;
        struct transport_list;
    } // nal

    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    namespace als {
        class animation_logic_system_interface;
        struct als_res_data;
    } // als

    class ai_core;
    class animation_controller;
    class capsule_alter_sys;
    struct resource_key;

    struct base_ai_data {
        u8       reserved_000[0x28];
        ai_core* core;
    };

    struct actor_unk_0b0 {
        u8                          reserved_000[0x0C];
        chuck::vm::script_instance* instance; // the actor's "__enx" instance
        u8*                         unk_010;  // its script object's name hash sits at +4
        resource_key*               unk_014;  // the script's resource; its hash is the filename
        string_hash                 unk_018;  // key prefix
        u8                          reserved_01c[0x04];
        als::als_res_data*          als_data;
    };

    class actor : public entity {

    public:
        u8                          reserved_074[0x30];
        nal::transport_list*        m_skeleton;
        nal::skeleton_map*          skeleton_map;
        animation_controller*       anim_ctrl;
        actor_unk_0b0*              unk_0b0;
        u8                          reserved_0b4[0x0C];
        amalga::resource_pack_slot* unk_0c0; // the pack slot the actor came from; script threads take it as unk_60
        u8                          reserved_0c4[0x0C];
        base_ai_data*               my_base_ai_data;
        capsule_alter_sys*          capsule_alter;

        ai_core*                               get_ai_core() const;
        als::animation_logic_system_interface* get_als() const;

        // slot 137
        void allocate_anim_controller() {
            ((void (__thiscall*)(actor*))vtable[137])(this);
        }

        // slot 156; adds to the movement the actor takes this frame
        void set_frame_delta_trans(const vector3d &delta, f32 delta_t) {
            ((void (__thiscall*)(actor*, const vector3d*, f32))vtable[156])(this, &delta, delta_t);
        }
    };

    ASSERT_OFFSETOF(base_ai_data, core, 0x28);

    ASSERT_OFFSETOF(actor_unk_0b0, instance, 0x0C);
    ASSERT_OFFSETOF(actor_unk_0b0, unk_018,  0x18);
    ASSERT_OFFSETOF(actor_unk_0b0, als_data, 0x20);

    ASSERT_OFFSETOF(actor, m_skeleton,      0xA4);
    ASSERT_OFFSETOF(actor, skeleton_map,    0xA8);
    ASSERT_OFFSETOF(actor, anim_ctrl,       0xAC);
    ASSERT_OFFSETOF(actor, unk_0b0,         0xB0);
    ASSERT_OFFSETOF(actor, unk_0c0,         0xC0);
    ASSERT_OFFSETOF(actor, my_base_ai_data, 0xD0);
    ASSERT_OFFSETOF(actor, capsule_alter,   0xD4);
} // treyarch
