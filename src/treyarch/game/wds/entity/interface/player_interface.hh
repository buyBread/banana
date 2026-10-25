#pragma once

#include "treyarch/game/wds/entity/interface/generic_interface.hh"
#include "treyarch/game/wds/entity/interface/player_reflexes.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class player_interface;

    using player_interface_method         = void (__thiscall*)(player_interface* self);
    using player_interface_advance_method = void (__thiscall*)(player_interface* self, f32 delta_t);

    /* slots 19+ are player_interface's own. SM3's order holds around frame_advance
       (map_controls 89, update_interface 91, frame_advance 92), but WoS inserted slots: SM3 has two getters at 19/20
       and a single post_process where retail has 93 and 94, so those are named from their retail use */
    struct player_interface_vtable : generic_interface_vtable {
        bool                            (__thiscall* is_enabled)(const player_interface* self);
        void                            (__thiscall* set_enabled)(player_interface* self, bool enabled);
        void*                           reserved_slots_021[68];
        bool                            (__thiscall* map_controls)(player_interface* self, i32 config, bool force);
        player_interface_method         render_phase;           // entity::invoke_render_phase
        player_interface_method         update_interface;
        player_interface_advance_method frame_advance;          // sub_9978B0
        player_interface_advance_method frame_advance_disabled; // sub_992120; clears input/swing state, ignores delta_t
        player_interface_advance_method unk_094;                // nullsub in player_interface; spiderman_rvb_player_interface overrides it
    };

    ASSERT_SIZEOF(player_interface_vtable, 95 * 4);

    class player_interface : public actor_interface {

    public:
        u8                   reserved_00c[0x247];
        /* cleared by game::freeze_hero on the first freeze and set again on the last unfreeze and by unload_current_level;
           world_dynamics_system::frame_advance runs frame_advance while set and frame_advance_disabled otherwise */
        bool                 enabled;
        u8                   reserved_254[0x180];
        player_reflexes      reflexes;
        player_spider_senses spider_senses;
        u8                   reserved_414[0x9C];
        arch_base_vhandle    unk_4b0; // an actor the als's target_ai rules read as the hero's target

        player_interface_vtable* get_vtable() const {
            return (player_interface_vtable*)vtable;
        }

        bool is_enabled() const {
            return get_vtable()->is_enabled(this);
        }

        void set_enabled(bool value) {
            get_vtable()->set_enabled(this, value);
        }

        void render_phase() {
            get_vtable()->render_phase(this);
        }

        void frame_advance(f32 delta_t) {
            get_vtable()->frame_advance(this, delta_t);
        }

        void frame_advance_disabled(f32 delta_t) {
            get_vtable()->frame_advance_disabled(this, delta_t);
        }

        void unk_094(f32 delta_t) {
            get_vtable()->unk_094(this, delta_t);
        }

        player_reflexes* get_reflexes() {
            return &reflexes;
        }

        player_spider_senses* get_spider_senses() {
            return &spider_senses;
        }
    };

    ASSERT_OFFSETOF(player_interface, enabled,       0x253);
    ASSERT_OFFSETOF(player_interface, reflexes,      0x3D4);
    ASSERT_OFFSETOF(player_interface, spider_senses, 0x3FC);
    ASSERT_OFFSETOF(player_interface, unk_4b0,       0x4B0);
} // treyarch
