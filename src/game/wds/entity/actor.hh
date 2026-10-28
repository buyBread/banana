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

    struct advanced_entity_ptrs {
        u8                          reserved_000[0x0C];
        chuck::vm::script_instance* my_script;       // the actor's "__enx" instance
        u8*                         unk_010;         // its script object's name hash sits at +4
        resource_key*               script_exe_name; // its hash is the script's filename
        string_hash                 script_obj_name;
        u8                          reserved_01c[0x04];
        als::als_res_data*          my_als_data;
        u8                          reserved_024[0x04];
        i32                         unk_028;         // set_really_immobile makes it 10, or 0
    };

    class actor : public entity {

    public:
        u8                          reserved_074[0x30];
        nal::transport_list*        m_skeleton;
        nal::skeleton_map*          skeleton_map;
        animation_controller*       anim_ctrl;
        advanced_entity_ptrs*       adv_ptrs;
        collision_geometry*         colgeom;
        u8                          reserved_0b8[0x08];
        amalga::resource_pack_slot* pack_slot; // the one the actor came from; its script threads take it as their resource context
        u8                          reserved_0c4[0x0C];
        base_ai_data*               my_base_ai_data;
        capsule_alter_sys*          capsule_alter;

        ai_core*                               get_ai_core() const;
        als::animation_logic_system_interface* get_als() const;

        // sub_602850
        bool has_base_ai_data() const {
            return my_base_ai_data != nullptr;
        }

        // slot 136
        bool is_a_rocket() const {
            return ((bool (__thiscall*)(const actor*))vtable[136])(this);
        }

        // slot 137
        void allocate_anim_controller() {
            ((void (__thiscall*)(actor*))vtable[137])(this);
        }

        // slot 144; null takes the abs po
        void update_colgeom(po* replacement_po = nullptr) {
            ((void (__thiscall*)(actor*, po*))vtable[144])(this, replacement_po);
        }

        // slot 155
        void set_frame_delta(const po &bob, f32 t) {
            ((void (__thiscall*)(actor*, const po*, f32))vtable[155])(this, &bob, t);
        }

        // slot 156; adds to the movement the actor takes this frame
        void set_frame_delta_trans(const vector3d &delta, f32 delta_t) {
            ((void (__thiscall*)(actor*, const vector3d*, f32))vtable[156])(this, &delta, delta_t);
        }

        // slot 157
        void invalidate_frame_delta() {
            ((void (__thiscall*)(actor*))vtable[157])(this);
        }

        void set_frame_delta_no_update(const po &bob, f32 t);
        void save_last_collision_free_state();

        static string_hash my_type_key;
    };

    ASSERT_OFFSETOF(base_ai_data, core, 0x28);

    ASSERT_OFFSETOF(advanced_entity_ptrs, my_script,       0x0C);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_exe_name, 0x14);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_obj_name, 0x18);
    ASSERT_OFFSETOF(advanced_entity_ptrs, my_als_data,     0x20);
    ASSERT_OFFSETOF(advanced_entity_ptrs, unk_028,         0x28);

    ASSERT_OFFSETOF(actor, m_skeleton,      0xA4);
    ASSERT_OFFSETOF(actor, skeleton_map,    0xA8);
    ASSERT_OFFSETOF(actor, anim_ctrl,       0xAC);
    ASSERT_OFFSETOF(actor, adv_ptrs,        0xB0);
    ASSERT_OFFSETOF(actor, colgeom,         0xB4);
    ASSERT_OFFSETOF(actor, pack_slot,       0xC0);
    ASSERT_OFFSETOF(actor, my_base_ai_data, 0xD0);
    ASSERT_OFFSETOF(actor, capsule_alter,   0xD4);
} // treyarch
