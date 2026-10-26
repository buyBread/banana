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
        void*                           reserved_slots_021[40];
        arch_base_vhandle*              (__thiscall* unk_061)(player_interface* self, arch_base_vhandle* result); // rvb: its +0x728; a null handle in player_interface
        void*                           reserved_slots_062[27];
        bool                            (__thiscall* map_controls)(player_interface* self, i32 config, bool force);
        player_interface_method         render_phase;           // entity::invoke_render_phase
        player_interface_method         update_interface;
        player_interface_advance_method frame_advance;          // sub_9978B0
        player_interface_advance_method frame_advance_disabled; // sub_992120; clears input/swing state, ignores delta_t
        player_interface_advance_method unk_094;                // nullsub in player_interface; spiderman_rvb_player_interface overrides it
        vector3d*                       (__thiscall* unk_095)(player_interface* self); // its own +0x2F0; the als takes it as a player's velocity
        void*                           reserved_slots_096[52];
        f32                             (__thiscall* unk_148)(player_interface* self); // the als's active_jump_charge
        f32                             (__thiscall* unk_149)(player_interface* self); // the als's jump_charge
        bool                            (__thiscall* unk_150)(player_interface* self); // the als's blacksuit_swing_window
        void*                           reserved_slots_151[1];
        bool                            (__thiscall* unk_152)(player_interface* self); // the als's trick_window
        void*                           reserved_slots_153[1];
        i32                             (__thiscall* unk_154)(player_interface* self); // the als's max_trick_combo
        void*                           reserved_slots_155[219];
        f32                             (__thiscall* unk_374)(player_interface* self); // the als's swing_time
        void*                           reserved_slots_375[2];
        bool                            (__thiscall* unk_377)(player_interface* self, vector3d* point); // rvb: the anchor of whichever of its two hands (+0x638) holds one; false in player_interface
        void*                           reserved_slots_378[9];
        void*                           (__thiscall* unk_387)(player_interface* self); // rvb: the pole it swings on (+0x8CC); null in player_interface
        bool                            (__thiscall* unk_388)(player_interface* self); // rvb: that pole leans more than 45 degrees off vertical
        void*                           reserved_slots_389[12];
        i32                             (__thiscall* unk_401)(player_interface* self); // the als's swing_hand
        void*                           reserved_slots_402[1];
        f32                             (__thiscall* unk_403)(player_interface* self); // the als's swing_level_rating
        void*                           reserved_slots_404[21];
        f32                             (__thiscall* unk_425)(player_interface* self); // the als's swing_angle, in degrees
        void*                           reserved_slots_426[1];
        f32                             (__thiscall* unk_427)(player_interface* self); // the als's swing_side_angle, in degrees
        void*                           reserved_slots_428[1];
        f32                             (__thiscall* unk_429)(player_interface* self); // the als's swing_to_anchor_angle, in degrees
        void*                           reserved_slots_430[2];
        f32                             (__thiscall* unk_432)(player_interface* self); // the als's swing_entry_speed
        f32                             (__thiscall* unk_433)(player_interface* self); // the als's swing_exit_speed
        void*                           reserved_slots_434[33];
        void*                           (__thiscall* unk_467)(player_interface* self); // rvb: clears +0xA44's first byte and returns it, a handle at +4; null in player_interface
        void*                           reserved_slots_468[5];
        arch_base_vhandle*              (__thiscall* unk_473)(player_interface* self, arch_base_vhandle* result); // its +0x498
    };

    ASSERT_SIZEOF(player_interface_vtable, 474 * 4);

    class player_interface : public actor_interface {

    public:
        u8                   reserved_00c[0x238];
        i32                  unk_244;        // the als counts 21 and 22 as carrying
        u8                   reserved_248[0x0B];
        /* cleared by game::freeze_hero on the first freeze and set again on the last unfreeze and by unload_current_level;
           world_dynamics_system::frame_advance runs frame_advance while set and frame_advance_disabled otherwise */
        bool                 enabled;
        u8                   reserved_254[0x4C];
        vector3d             unk_2a0;        // als::crawl_transition's destination without attach_point
        u8                   reserved_2ac[0x128];
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

        arch_base_vhandle unk_061() {
            arch_base_vhandle result;
            get_vtable()->unk_061(this, &result);

            return result;
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

        vector3d* unk_095() {
            return get_vtable()->unk_095(this);
        }

        f32 unk_148() {
            return get_vtable()->unk_148(this);
        }

        f32 unk_149() {
            return get_vtable()->unk_149(this);
        }

        bool unk_150() {
            return get_vtable()->unk_150(this);
        }

        bool unk_152() {
            return get_vtable()->unk_152(this);
        }

        i32 unk_154() {
            return get_vtable()->unk_154(this);
        }

        f32 unk_374() {
            return get_vtable()->unk_374(this);
        }

        bool unk_377(vector3d* point) {
            return get_vtable()->unk_377(this, point);
        }

        void* unk_387() {
            return get_vtable()->unk_387(this);
        }

        bool unk_388() {
            return get_vtable()->unk_388(this);
        }

        i32 unk_401() {
            return get_vtable()->unk_401(this);
        }

        f32 unk_403() {
            return get_vtable()->unk_403(this);
        }

        f32 unk_425() {
            return get_vtable()->unk_425(this);
        }

        f32 unk_427() {
            return get_vtable()->unk_427(this);
        }

        f32 unk_429() {
            return get_vtable()->unk_429(this);
        }

        f32 unk_432() {
            return get_vtable()->unk_432(this);
        }

        f32 unk_433() {
            return get_vtable()->unk_433(this);
        }

        void* unk_467() {
            return get_vtable()->unk_467(this);
        }

        arch_base_vhandle unk_473() {
            arch_base_vhandle result;
            get_vtable()->unk_473(this, &result);

            return result;
        }

        player_reflexes* get_reflexes() {
            return &reflexes;
        }

        player_spider_senses* get_spider_senses() {
            return &spider_senses;
        }
    };

    ASSERT_OFFSETOF(player_interface, unk_244,       0x244);
    ASSERT_OFFSETOF(player_interface, enabled,       0x253);
    ASSERT_OFFSETOF(player_interface, unk_2a0,       0x2A0);
    ASSERT_OFFSETOF(player_interface, reflexes,      0x3D4);
    ASSERT_OFFSETOF(player_interface, spider_senses, 0x3FC);
    ASSERT_OFFSETOF(player_interface, unk_4b0,       0x4B0);
} // treyarch
