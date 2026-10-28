#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface/player_reflexes.hh"
#include "game/input/game_axis.hh"
#include "game/input/game_button.hh"
#include "game/arch_base_vhandle.hh"
#include "game/delta_variable.hh"
#include "treyarch/mash/array_replacement.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_core;
    class entity;
    class player_axis_control;
    class player_interface;

    using player_interface_method         = void (__thiscall*)(player_interface* self);
    using player_interface_advance_method = void (__thiscall*)(player_interface* self, f32 delta_t);

    /* slots 19+ are player_interface's own. SM3's order holds around frame_advance
       (map_controls 89, update_interface 91, frame_advance 92), but WoS inserted slots: SM3 has two getters at 19/20
       and a single post_process where retail has 93 and 94, so those are named from their retail use */
    struct player_interface_vtable : generic_interface_vtable {
        bool                            (__thiscall* is_enabled)(const player_interface* self);
        void                            (__thiscall* set_enabled)(player_interface* self, bool enabled);
        void*                           reserved_slots_021[16];
        void                            (__thiscall* set_zip_override_anchor_num)(player_interface* self, f32 num); // nullsub in player_interface
        void*                           reserved_slots_038[23];
        arch_base_vhandle*              (__thiscall* unk_061)(player_interface* self, arch_base_vhandle* result); // rvb: its +0x728; a null handle in player_interface
        void*                           reserved_slots_062[13];
        void                            (__thiscall* set_desired_mode)(player_interface* self, i32 m);
        void*                           reserved_slots_076[2];
        void                            (__thiscall* set_swing_override_anchor)(player_interface* self, arch_base_vhandle anchor); // nullsub in player_interface
        void                            (__thiscall* set_zip_override_anchor)(player_interface* self, arch_base_vhandle anchor);   // nullsub in player_interface
        void*                           reserved_slots_080[9];
        bool                            (__thiscall* map_controls)(player_interface* self, i32 config, bool force);
        player_interface_method         render_phase;           // entity::invoke_render_phase
        player_interface_method         update_interface;
        player_interface_advance_method frame_advance;          // sub_9978B0
        player_interface_advance_method frame_advance_disabled; // sub_992120; clears input/swing state, ignores delta_t
        player_interface_advance_method unk_094;                // nullsub in player_interface; spiderman_rvb_player_interface overrides it
        vector3d*                       (__thiscall* unk_095)(player_interface* self); // its own +0x2F0; the als takes it as a player's velocity
        void*                           reserved_slots_096[24];
        bool                            (__thiscall* crawling)(const player_interface* self);
        void*                           reserved_slots_121[19];
        player_interface_method         unk_140;                // the snap and translate natives call it after a move
        void*                           reserved_slots_141[7];
        f32                             (__thiscall* unk_148)(player_interface* self); // the als's active_jump_charge
        f32                             (__thiscall* unk_149)(player_interface* self); // the als's jump_charge
        bool                            (__thiscall* unk_150)(player_interface* self); // the als's blacksuit_swing_window
        void*                           reserved_slots_151[1];
        bool                            (__thiscall* unk_152)(player_interface* self); // the als's trick_window
        void*                           reserved_slots_153[1];
        i32                             (__thiscall* unk_154)(player_interface* self); // the als's max_trick_combo
        void*                           reserved_slots_155[127];
        bool                            (__thiscall* unk_282)(player_interface* self, ai_core* core); // drops what the core's carry inode holds
        void*                           reserved_slots_283[89];
        entity*                         (__thiscall* get_last_anchor)(const player_interface* self); // null in both player_interface and rvb
        void*                           reserved_slots_373[1];
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
        void*                           reserved_slots_474[12];
        void                            (__thiscall* unk_486)(player_interface* self, bool unk, f32 unk_2); // rvb: stores both at +0xB5C and +0xB54; nullsub in player_interface
    };

    ASSERT_SIZEOF(player_interface_vtable, 487 * 4);

    class player_interface : public actor_interface {

    public:
        u8                    reserved_00c[0x234];
        delta_variable<i32>   mode;
        i32                   mode_last_frame;
        i32                   desired_mode;
        u8                    reserved_250[0x03];
        /* cleared by game::freeze_hero on the first freeze and set again on the last unfreeze and by unload_current_level;
           world_dynamics_system::frame_advance runs frame_advance while set and frame_advance_disabled otherwise */
        bool                  enabled;
        i32                   control_config;
        game_axis*            control_axis[4];
        player_axis_control*  player_axis[3];
        u8                    reserved_274[0x2C];
        vector3d              unk_2a0;       // als::crawl_transition's destination without attach_point
        u8                    reserved_2ac[0x128];
        player_reflexes       reflexes;
        player_spider_senses  spider_senses;
        u8                    reserved_414[0x9C];
        arch_base_vhandle     unk_4b0;       // an actor the als's target_ai rules read as the hero's target
        u8                    reserved_4b4[0x4C];
        bool                  override_mode;
        u8                    reserved_501[0x1B];
        mash::array_replacement
            <game_button, 25> buttons;
        mash::array_replacement
            <game_button, 89> command_buttons;

        player_interface_vtable* get_vtable() const {
            return (player_interface_vtable*)vtable;
        }

        bool is_enabled() const {
            return get_vtable()->is_enabled(this);
        }

        void set_enabled(bool value) {
            get_vtable()->set_enabled(this, value);
        }

        void set_zip_override_anchor_num(f32 num) {
            get_vtable()->set_zip_override_anchor_num(this, num);
        }

        arch_base_vhandle unk_061() {
            arch_base_vhandle result;
            get_vtable()->unk_061(this, &result);

            return result;
        }

        void set_desired_mode(i32 m) {
            get_vtable()->set_desired_mode(this, m);
        }

        void set_swing_override_anchor(arch_base_vhandle anchor) {
            get_vtable()->set_swing_override_anchor(this, anchor);
        }

        void set_zip_override_anchor(arch_base_vhandle anchor) {
            get_vtable()->set_zip_override_anchor(this, anchor);
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

        bool crawling() const {
            return get_vtable()->crawling(this);
        }

        void unk_140() {
            get_vtable()->unk_140(this);
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

        bool unk_282(ai_core* core) {
            return get_vtable()->unk_282(this, core);
        }

        entity* get_last_anchor() const {
            return get_vtable()->get_last_anchor(this);
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

        void unk_486(bool unk, f32 unk_2) {
            get_vtable()->unk_486(this, unk, unk_2);
        }

        i32 get_mode() const {
            return mode.current;
        }

        i32 get_prev_mode() const {
            return mode.previous;
        }

        void enable_axis(i32 ctrl, bool v) {
            control_axis[ctrl]->enable(v);
        }

        game_button* get_button(i32 ctrl) {
            return &buttons[ctrl];
        }

        // sub_783720
        // inlined @ sub_86B470
        void enable_button(i32 ctrl, bool v) {
            buttons[ctrl].enable(v);
        }

        game_button* get_command_button(i32 cmd) {
            return &command_buttons[cmd];
        }

        void clear_buttons();
        void clear_controller(bool unk);
        void override_button(i32 ctrl, f32 value, f32 t);
        void override_command(i32 cmd, f32 value, f32 t);
        void set_button(i32 ctrl, f32 value);
        void set_axis(i32 ctrl, bool ud, f32 value);

        player_reflexes* get_reflexes() {
            return &reflexes;
        }

        player_spider_senses* get_spider_senses() {
            return &spider_senses;
        }
    };

    ASSERT_OFFSETOF(player_interface, mode,            0x240);
    ASSERT_OFFSETOF(player_interface, desired_mode,    0x24C);
    ASSERT_OFFSETOF(player_interface, enabled,         0x253);
    ASSERT_OFFSETOF(player_interface, control_config,  0x254);
    ASSERT_OFFSETOF(player_interface, control_axis,    0x258);
    ASSERT_OFFSETOF(player_interface, player_axis,     0x268);
    ASSERT_OFFSETOF(player_interface, unk_2a0,         0x2A0);
    ASSERT_OFFSETOF(player_interface, reflexes,        0x3D4);
    ASSERT_OFFSETOF(player_interface, spider_senses,   0x3FC);
    ASSERT_OFFSETOF(player_interface, unk_4b0,         0x4B0);
    ASSERT_OFFSETOF(player_interface, override_mode,   0x500);
    ASSERT_OFFSETOF(player_interface, buttons,         0x51C);
    ASSERT_OFFSETOF(player_interface, command_buttons, 0x520);
} // treyarch
