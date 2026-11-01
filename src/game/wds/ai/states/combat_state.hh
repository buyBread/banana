#pragma once

#include "game/audio/sound_response.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/param_block.hh"
#include "game/als/als.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/float_spring.hh"
#include "game/resource_key.hh"
#include "game/timing/simple_timer.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mash/array_replacement.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }}

    class actor;
    class als_inode;
    class combat_state;
    class cpu_combat_inode;
    class entity;
    class entity_base;
    class plr_strength_test;
    class weapon_interface_extender;
    struct hit_react_record;

    // SM3's eCombatMove; WoS has more moves than SM3
    enum e_combat_move : i32 {
        combat_move_light_melee         = 0x00,
        combat_move_heavy_melee         = 0x01,
        combat_move_special_attack      = 0x02,
        combat_move_jump_attack         = 0x03,
        combat_move_bounce_attack       = 0x04,
        combat_move_counter_attack      = 0x05,
        combat_move_dodge               = 0x06,
        combat_move_block               = 0x07,
        combat_move_save_civ            = 0x08,
        combat_move_light_ranged        = 0x09,
        combat_move_heavy_ranged        = 0x0A,
        combat_move_grapple             = 0x0B,
        combat_move_throw_manip         = 0x0C,
        combat_move_grab_civ            = 0x0D,
        combat_move_boss_finisher       = 0x0E,
        combat_move_car_finisher        = 0x0F,
        combat_move_boost_power_attack  = 0x10,
        combat_move_boost_speed_attack  = 0x11,
        combat_move_boost_air_attack    = 0x12,
        combat_move_boost_web_attack    = 0x13,
        combat_move_manual_dodge        = 0x14,
        combat_move_script_move         = 0x15,
        combat_move_momentum_light      = 0x16,
        combat_move_momentum_heavy      = 0x17,
        combat_move_momentum_grapple    = 0x18,
        combat_move_stealth_kill        = 0x19,
        combat_move_swing_light         = 0x1A,
        combat_move_swing_heavy         = 0x1B,
        combat_move_swing_grapple       = 0x1C,
        combat_move_light_wall_attack   = 0x1D,
        combat_move_heavy_wall_attack   = 0x1E,
        combat_move_enemy_swing_light   = 0x1F,
        combat_move_enemy_swing_heavy   = 0x20,
        combat_move_enemy_swing_grapple = 0x21,
        combat_move_pole_swing_light    = 0x22,
        combat_move_pole_swing_heavy    = 0x23,
        combat_move_pole_swing_grapple  = 0x24,
        combat_move_grab_obj            = 0x25,
        combat_num_moves                = 0x26, // no move
        combat_max_moves                = 1000,
        combat_move_trick_a             = 1001,
        combat_move_trick_b             = 1002,
        combat_move_trick_c             = 1003,
        combat_move_trick_d             = 1004,
        combat_and_tricks_num_moves     = 1005  // no trick
    };

    // the script library's move order
    f32 get_combat_move_script_code(e_combat_move move, i32 script_move);

    // SM3's combat_state::eCombatSignal and WoS's additions
    enum e_combat_signal : u32 {
        combat_signal_anim_action         = 0x00000001,
        combat_signal_attack_begin        = 0x00000002,
        combat_signal_attack_end          = 0x00000004,
        combat_signal_attack              = 0x00000008,
        combat_signal_web_shoot           = 0x00000010,
        combat_signal_web_release         = 0x00000020,
        combat_signal_animated_camera_on  = 0x00000040,
        combat_signal_animated_camera_off = 0x00000080,
        combat_signal_move_begin          = 0x00000100,
        combat_signal_move_end            = 0x00000200,
        combat_signal_rotate_begin        = 0x00000400,
        combat_signal_rotate_end          = 0x00000800,
        combat_signal_translate_begin     = 0x00001000,
        combat_signal_translate_end       = 0x00002000
    };

    // SM3's combo_flags_1
    enum e_combo_flags_1 : u32 {
        combo_flag_explosive                            = 0x00000001,
        combo_flag_area_of_effect                       = 0x00000002,
        combo_flag_maintain_prev_targets                = 0x00000004,
        combo_flag_non_signal                           = 0x00000008,
        combo_flag_always_jump                          = 0x00000010,
        combo_flag_ignore_standing                      = 0x00000020,
        combo_flag_delay_stun_recovery                  = 0x00000040,
        combo_flag_ignore_target_collisions             = 0x00000080,
        combo_flag_recenter_camera                      = 0x00000100,
        combo_flag_invulnerable_until_attack            = 0x00000200,
        combo_flag_combo_breaker                        = 0x00000400,
        combo_flag_unbreakable                          = 0x00000800,
        combo_flag_allow_flight_thrusters               = 0x00001000,
        combo_flag_self_connect_cancel_all_velocity     = 0x00002000,
        combo_flag_target_connect_cancel_all_velocity   = 0x00004000,
        combo_flag_self_cancel_velocity_dir_both_ways   = 0x00008000,
        combo_flag_target_cancel_velocity_dir_both_ways = 0x00010000,
        combo_flag_equalize_velocity                    = 0x00020000,
        combo_flag_use_target_gravity                   = 0x00040000,
        combo_flag_clear_combo_count                    = 0x00080000,
        combo_flag_jump_connect_only_if_button_held     = 0x00100000,
        combo_flag_instant_kill                         = 0x00200000,
        combo_flag_auto_kill                            = 0x00400000,
        combo_flag_instant_stun                         = 0x00800000,
        combo_flag_auto_stun                            = 0x01000000,
        combo_flag_dodge                                = 0x02000000,
        combo_flag_always_hit                           = 0x04000000,
        combo_flag_invulnerable                         = 0x08000000,
        combo_flag_non_interruptable                    = 0x10000000,
        combo_flag_counter_attack                       = 0x20000000,
        combo_flag_exit_to_crawl                        = 0x40000000,
        combo_flag_exit_to_jump                         = 0x80000000
    };

    // SM3's combo_flags_2
    enum e_combo_flags_2 : u32 {
        combo_flag_maintain_script_obj          = 0x00000001,
        combo_flag_awe_inspiring                = 0x00000002,
        combo_flag_primary_target_movement      = 0x00000004,
        combo_flag_sync_anim                    = 0x00000008,
        combo_flag_strength_test                = 0x00000010,
        combo_flag_auto_draw_weapon             = 0x00000020,
        combo_flag_auto_holster_weapon          = 0x00000040,
        combo_flag_kill_sync_anim_on_hit        = 0x00000080,
        combo_flag_allow_target_dodge           = 0x00000100,
        combo_flag_force_add_incoming_attack    = 0x00000200,
        combo_flag_maintain_standing            = 0x00000400,
        combo_flag_use_weapon_gun               = 0x00000800,
        combo_flag_use_weapon_grenade           = 0x00001000,
        combo_flag_use_weapon_rocket            = 0x00002000,
        combo_flag_use_weapon_melee             = 0x00004000,
        combo_flag_use_weapon_if_empty          = 0x00008000,
        combo_flag_use_weapon_2_gun             = 0x00010000,
        combo_flag_use_weapon_2_grenade         = 0x00020000,
        combo_flag_use_weapon_2_rocket          = 0x00040000,
        combo_flag_use_weapon_2_melee           = 0x00080000,
        combo_flag_use_weapon_2_if_empty        = 0x00100000,
        combo_flag_buddy_combo                  = 0x00200000,
        combo_flag_cancel_target_death_immunity = 0x00400000,
        combo_flag_cancel_target_stun           = 0x00800000,
        combo_flag_dodge_counts_as_hit          = 0x01000000,
        combo_flag_kill_buddy_combo_on_hit      = 0x02000000,
        combo_flag_allow_counter_attack         = 0x04000000,
        combo_flag_holster_weapon_on_attack     = 0x08000000,
        combo_flag_holster_weapon_2_on_attack   = 0x10000000,
        combo_flag_reset_double_jump            = 0x20000000,
        combo_flag_block                        = 0x40000000,
        combo_flag_allow_target_block           = 0x80000000
    };

    // SM3's combo_flags_3 and WoS's additions
    enum e_combo_flags_3 : u32 {
        combo_flag_ignore_add_incoming_attack         = 0x00000001,
        combo_flag_animated_camera                    = 0x00000002,
        combo_flag_animated_camera_collide_with_world = 0x00000004,
        combo_flag_strength_test_looping_anim         = 0x00000008,
        combo_flag_apply_dilation_on_start            = 0x00000010,
        combo_flag_allow_vertical_movement            = 0x00000020,
        combo_flag_disable_standing                   = 0x00000040,
        combo_flag_disable_standing_until_attack      = 0x00000080,
        combo_flag_apply_jump_launch_on_action        = 0x00000100,
        combo_flag_3_unk_00000200                     = 0x00000200, // enter_attack_window clears standing flag 0x1000
        combo_flag_3_unk_00000400                     = 0x00000400, // the move launches the actor (combat_state slots 56 and 60)
        combo_flag_3_unk_00001000                     = 0x00001000, // enter_attack_window clears physical flag 0x200
        combo_flag_3_unk_00020000                     = 0x00020000, // a transition into the move re-reads initially_standing
        combo_flag_3_unk_00800000                     = 0x00800000, // a transition into the move picks its targets again
        combo_flag_3_unk_04000000                     = 0x04000000  // so does this one
    };

    enum e_combo_unk_10_flags : u32 {
        combo_unk_10_flag_00000010 = 0x00000010, // the move keeps the actor's own up vector
        combo_unk_10_flag_00000040 = 0x00000040, // the move can end in a ground splat
        combo_unk_10_flag_00000200 = 0x00000200, // slot 26 builds the up from the direction to the targets
        combo_unk_10_flag_00000400 = 0x00000400, // slot 26 takes +Y as the up
        combo_unk_10_flag_00008000 = 0x00008000, // the hero raises WALL_HIT on what he hits
        combo_unk_10_flag_00010000 = 0x00010000, // AIR_HIT
        combo_unk_10_flag_00020000 = 0x00020000, // GROUND_HIT
        combo_unk_10_flag_00100000 = 0x00100000, // slot 26 takes the first target's up
        combo_unk_10_flag_00200000 = 0x00200000, // no damage to a friendly ai; the hero picks it up
        combo_unk_10_flag_00800000 = 0x00800000  // the move looks for a wall ahead
    };

    // the hero's combat frame hands the player interface slot beside it most of these
    enum e_combo_unk_14_flags : u32 {
        combo_unk_14_flag_00000001 = 0x00000001, // exit_attack_window clears standing flag 0x800
        combo_unk_14_flag_00000002 = 0x00000002, // exit_attack_window clears standing flag 0x1000
        combo_unk_14_flag_00000004 = 0x00000004, // exit_attack_window clears physical flag 0x200
        combo_unk_14_flag_00000040 = 0x00000040, // slot 492, only when set; hit_target hands slot 514 the target
        combo_unk_14_flag_00000200 = 0x00000200, // slot 506, only when set
        combo_unk_14_flag_00000400 = 0x00000400, // slot 175
        combo_unk_14_flag_00010000 = 0x00010000, // slot 512, only when set
        combo_unk_14_flag_00800000 = 0x00800000, // slot 173
        combo_unk_14_flag_01000000 = 0x01000000, // slot 171
        combo_unk_14_flag_02000000 = 0x02000000, // slot 169
        combo_unk_14_flag_04000000 = 0x04000000  // slot 167
    };

    enum e_combo_unk_18_flags : u32 {
        combo_unk_18_flag_00000001 = 0x00000001, // the hero's combat frame hands it to player slot 177
        combo_unk_18_flag_00000002 = 0x00000002, // slot 179
        combo_unk_18_flag_00000008 = 0x00000008, // the launch runs every frame, not only on an anim action
        combo_unk_18_flag_00000010 = 0x00000010, // the hero's hit adds nothing to meter 0
        combo_unk_18_flag_00000400 = 0x00000400, // hit_target doesn't record the target in already_hit_targets
        combo_unk_18_flag_00001000 = 0x00001000, // the attack window's first target hit becomes the primary target
        combo_unk_18_flag_00008000 = 0x00008000  // a transition into the move clears combat_state::hit_dilation_pending
    };

    // SM3's CombatAnimValues; WoS has twelve signals of three elements, the third the time to the signal over the layer's speeds
    enum e_combat_anim_values : u32 {
        cmbt_action_signal_raised            = 0,
        cmbt_time_to_action_signal           = 1,
        cmbt_attack_signal_raised            = 3,
        cmbt_time_to_attack_signal           = 4,
        cmbt_attack_begin_signal_raised      = 6,
        cmbt_time_to_attack_begin_signal     = 7,
        cmbt_attack_end_signal_raised        = 9,
        cmbt_time_to_attack_end_signal       = 10,
        cmbt_web_shoot_signal_raised         = 12,
        cmbt_time_to_web_shoot_signal        = 13,
        cmbt_web_release_signal_raised       = 15,
        cmbt_time_to_web_release_signal      = 16,
        cmbt_move_begin_signal_raised        = 18,
        cmbt_time_to_move_begin_signal       = 19,
        cmbt_move_end_signal_raised          = 21,
        cmbt_time_to_move_end_signal         = 22,
        cmbt_rotate_begin_signal_raised      = 24,
        cmbt_time_to_rotate_begin_signal     = 25,
        cmbt_rotate_end_signal_raised        = 27,
        cmbt_time_to_rotate_end_signal       = 28,
        cmbt_translate_begin_signal_raised   = 30,
        cmbt_time_to_translate_begin_signal  = 31,
        cmbt_translate_end_signal_raised     = 33,
        cmbt_time_to_translate_end_signal    = 34,
        cmbt_anim_time                       = 36,
        cmbt_anim_duration                   = 37,
        cmbt_normalized_anim_time            = 38,
        cmbt_damage                          = 39,
        cmbt_damage_type                     = 40,
        cmbt_button_jump_triggered           = 41,
        cmbt_button_light_melee_triggered    = 42,
        cmbt_button_heavy_melee_triggered    = 43,
        cmbt_button_grapple_triggered        = 44,
        cmbt_button_light_ranged_triggered   = 45,
        cmbt_button_held                     = 46,
        cmbt_unk_47                          = 47, // always -1
        cmbt_script_move_id                  = 48,
        cmbt_strength_test_normalized_amount = 49,
        cmbt_unk_50                          = 50, // the frame's move, as the script library codes it
        num_combat_anim_values
    };

    // one move of a combat chain
    class combo_move_data : public param_block_creatable_class {

    public:
        u32         flags_1;
        u32         flags_2;
        u32         flags_3;
        u32         unk_10;
        u32         unk_14;
        u32         unk_18;
        u8          reserved_1c[0x04];
        param_block params;

        const char*      get_script_exe                (bool* exists) const;
        const char*      get_script_obj                (bool* exists) const;
        f32              get_script_move_id            (bool* exists) const;
        i32              get_max_targets               (bool* exists) const;
        f32              get_jump_launch_force         (bool* exists) const;
        f32              get_strength_test_start_value (bool* exists) const;
        als::category_id get_target_sync_category      (bool* exists) const;
        als::category_id get_buddy_sync_category       (bool* exists) const;
        i32              get_combo_move_type_id        (bool* exists) const;
        string_hash      get_combo_move_name           (bool* exists) const;
        f32              get_combo_counter_value       (bool* exists) const;
        f32              get_combo_counter_delay       (bool* exists) const;
        const char*      get_dodge_anim                (bool* exists) const;
        i32              get_dodge_type                (bool* exists) const;
        f32              get_damage_arc                (bool* exists) const;
        f32              get_attack_time               (bool* exists) const;
        string_hash      get_wounded_category          (bool* exists) const;
        string_hash      get_attack_sound_group        (bool* exists) const;
        string_hash      get_missed_sound_group        (bool* exists) const;
        string_hash      get_dodged_sound_group        (bool* exists) const;
        vector3d         get_self_cancel_velocity_dir  (bool* exists) const;
        vector3d         get_target_cancel_velocity_dir(bool* exists) const;
        vector3d         get_auto_calc_targets_dir     (bool* exists) const;
    };

    // what internal_auto_calc_targets finds
    struct auto_calc_target {
        arch_base_vhandle target;
        u8                reserved_04[0x0C];
    };

    // a combat script's frame_advance thread and what its natives may ask about it
    struct combat_thread_info {
              chuck::vm::script_instance* the_instance;
              combat_state*               frame_advancing_combat_state;
              i32                         frame_advancing_chain_size;
        const combo_move_data*            frame_advancing_combo_move;
              cpu_combat_inode*           frame_advancing_cpu_combat_inode;
              weapon_interface_extender*  frame_advancing_weapon;
              entity_base*                frame_advancing_weapon_entity;
              i32                         frame_advancing_usage_type;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    // the melee state; abstract, its subclasses pick the moves
    class combat_state : public enhanced_state {

    public:
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        current_buddies;
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        current_targets;
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        unk_04c;                           // clear_targets swaps it with current_targets
        chuck::vm::script_dynamic_array
            <f32>*                      combat_anim_values;
        vc8::vector
            <arch_base_vhandle>*        already_hit_targets;
        arch_base_vhandle               throw_target;                      // the auto_calc_throw_target params pick it
        resource_key                    combat_script_exec_prefix;
        arch_base_vhandle               combat_script_inst;                // the instance's shadow
        i32                             init_move_func_id;
        i32                             frame_advance_func_id;
        i32                             hit_target_func_id;
        i32                             allow_target_hit_func_id;
        i32                             spawn_hit_fx_func_id;
        i32                             spawn_blocked_fx_func_id;
        i32                             spawn_hit_sound_func_id;
        i32                             spawn_attack_sound_func_id;
        i32                             spawn_missed_sound_func_id;
        i32                             spawn_dodged_sound_func_id;
        i32                             spawn_blocked_sound_func_id;
        i32                             spawn_attack_window_sound_func_id;
        sound_response                  unk_098;
        i32                             get_als_facing_func_id;
        i32                             get_als_up_func_id;
        i32                             get_als_position_func_id;
        i32                             get_jump_launch_vector_func_id;
        i32                             get_blast_vector_func_id;
        i32                             kill_move_func_id;
        i32                             custom_camera_func_id;
        i32                             custom_camera_engaged_func_id;
        i32                             combat_move_func_id;
        i32                             move_transition_func_id;
        i32                             strength_test_values_func_id;
        i32                             playback_speed_func_id;
        i32                             get_damage_func_id;
        i32                             get_damage_type_func_id;
        i32                             get_auto_calc_target_pos_func_id;
        i32                             get_auto_calc_target_dir_func_id;
        i32                             set_gun_fire_info_func_id;
        i32                             set_rocket_fire_info_func_id;
        i32                             set_grenade_fire_info_func_id;
        i32                             update_target_lists_func_id;
        e_state_exit_code               preferred_exit_status;
        f32                             recovery_time;
        vector3d                        fake_attack_face_dir;
        vector3d                        fake_attack_pos;
        u8                              reserved_118[0x24];
        i32                             chain_size;
        i32                             unk_140;                           // request-made transitions in a row that kept the first target
        u32                             signal_flags;                      // e_combat_signal
        u32                             callback_ids[20];                  // activate's event callbacks, removed by deactivate
        plr_strength_test*              strength_test;
        simple_timer                    strength_test_delay;
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        sync_targets;                      // the ones start_sync_anim engaged
        arch_base_vhandle               sync_buddy;
        i32                             next_weapon_index;                 // the one weapon a move that alternates fires next
        f32                             delay_standing;                    // the move's params, copied by move_transition
        f32                             target_exit_speed;
        vector3d                        target_exit_vector;
        f32                             jump_exit_speed;
        vector3d                        jump_exit_vector;
        u8                              reserved_1d0[0x04];
        bool                            unk_1d4;
        bool                            unk_1d5;
        boolx                           initially_standing;
        boolx                           in_attack_window;
        boolx                           attack_connected;
        boolx                           last_attack_connected;
        boolx                           attack_dodged;
        boolx                           last_attack_dodged;
        boolx                           attack_blocked;
        boolx                           last_attack_blocked;
        boolx                           attack_missed;
        boolx                           last_attack_missed;
        boolx                           attack_applied;
        boolx                           jump_connect_applied;
        bool                            jump_launch_applied;               // slot 56's answer, taken once per chain
        boolx                           delaying_stun;
        boolx                           in_custom_cam;
        boolx                           executed_faked_time_attack;
        boolx                           hero_is_a_target;
        boolx                           cancel_target_death_immunity;
        boolx                           auto_draw_move;
        boolx                           auto_holster_move;
        boolx                           in_sync_combo_anim;
        boolx                           in_buddy_combo_anim;
        bool                            attack_sound_spawned;              // cleared as a move starts
        u8                              reserved_1ed[0x03];
        bool                            hit_dilation_waits_for_signal;
        bool                            hit_dilation_single_signal;        // the first HIT_DILATION signal clears hit_dilation_pending
        bool                            hit_dilation_pending;
        u8                              pad_1f3[0x01];
        float_spring                    v_look;
        f32                             time_dilation_start_time;
        f32                             time_dilation_start_scale;
        f32                             time_dilation_end_time;
        f32                             time_dilation_end_scale;
        f32                             time_dilation_scale;
        i32                             time_dilation_mode;                // the move's param, copied by move_transition
        vector3d                        hit_sphere_pos[8];                 // per hit sphere of the base layer's state
        vector3d                        hit_sphere_prev_pos[8];
        bool                            hit_sphere_prev_valid[8];          // both attack window slots clear them
        bool                            attack_end_pending;                // ATTACK_END inside the window closes it the next frame
        u8                              pad_2f1[0x03];
        mash::array_replacement
            <string_hash, 2>            weapon_to_use;

        explicit combat_state(mash::generating_vtable_version so_do_nothing) : enhanced_state(so_do_nothing) {}

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;
        void              get_info_node_list(info_node_desc_list &the_list) const override;
        bool              is_combat_state() const override;
        u32               unk_019() const override;

        virtual void          update_next_move() = 0;
        virtual e_combat_move get_next_move(f32 delta_t, i32* next_script_move_num) = 0;
        virtual e_combat_move get_initial_move(i32* next_script_move_num);
        virtual void          move_transition(const combo_move_data* new_combo_move, u32 unk);
        virtual void          add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move);
        virtual void          set_als_facing(als_inode* my_als, const combo_move_data* combo_move);
        virtual void          unk_026(als_inode* my_als, const combo_move_data* combo_move); // the base layer's desired_up_vector
        virtual void          set_als_position(als_inode* my_als, const combo_move_data* combo_move);

        virtual void update_combat(f32               delta_t,
                                   e_combat_move     next_move_enum,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt);

        virtual void     fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t);
        virtual bool     is_combo_cancelled(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) const;
        virtual bool     unk_031(f32 fall_speed); // whether the fall hurts
        virtual void     clear_targets();
        virtual i32      get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const;
        virtual i32      get_damage(const combo_move_data* combo_move, arch_base_vhandle target) const;
        virtual vector3d unk_035(const combo_move_data* combo_move, arch_base_vhandle target);
        virtual void     enter_custom_camera(const combo_move_data* combo_move);
        virtual void     exit_custom_camera();
        virtual void     kill_sync_anim(bool next_move_is_synced);
        virtual void     start_sync_anim(const combo_move_data* combo_move);
        virtual void     kill_buddy_combo(bool next_move_is_buddy);
        virtual void     start_buddy_combo(const combo_move_data* combo_move);
        virtual void     kill_strength_test();

        virtual void start_strength_test(const combo_move_data*  combo_move,
                                               als_inode*        my_als,
                                               cpu_combat_inode* cmbt,
                                               f32               delta_t);

        virtual void spawn_sound(const combo_move_data* combo_move,
                                       entity*          player,
                                       string_hash      sound_grp,
                                 const vector3d*        connect_point,
                                       i32              sound_func_id,
                                       void*            unk);

        virtual void spawn_sound_impact(const combo_move_data* combo_move,
                                              entity*          attacker,
                                              entity*          target,
                                        const vector3d*        connect_point,
                                              i32              damage,
                                              bool             blocked);

        virtual string_hash   get_combat_als_category() const;
        virtual als::layer_id get_combat_layer();
        virtual bool          unk_048(const combo_move_data* combo_move, void* unk);

        virtual bool unk_049(      entity*           target,
                             const combo_move_data*  combo_move,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt,
                                   vector3d*         connect_point); // the hit test against the actor's reach, for a state without hit spheres

        virtual bool unk_050(      entity*           target,
                             const combo_move_data*  combo_move,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt,
                                   vector3d*         connect_point); // the hit test against the base layer state's hit spheres

        virtual bool unk_051(entity* target, const vector3d* target_point, const vector3d* attacker_point); // whether the world blocks the line between the points

        virtual bool check_for_target_hit(      entity*           target,
                                          const combo_move_data*  combo_move,
                                                als_inode*        my_als,
                                                cpu_combat_inode* cmbt,
                                                vector3d*         connect_point);

        virtual bool hit_target(      entity*           target,
                                const combo_move_data*  combo_move,
                                      als_inode*        my_als,
                                      cpu_combat_inode* cmbt,
                                      vector3d*         connect_point);

        virtual bool target_was_hit(arch_base_vhandle target);
        virtual bool apply_jump_connect(const combo_move_data* combo_move);
        virtual bool unk_056(const combo_move_data* combo_move);
        virtual void unk_057(const combo_move_data* combo_move, f32 unk);
        virtual void unk_058();
        virtual void unk_059(arch_base_vhandle target);
        virtual void unk_060(f32 unk, const combo_move_data* combo_move);
        virtual void enter_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt);
        virtual void exit_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt);
        virtual void unk_063(als_inode* my_als, entity* target);

        virtual bool check_for_dodge(      entity*                    target,
                                           i32                        chain_size,
                                     const combo_move_data*           combo_move,
                                           weapon_interface_extender* weapon,
                                           entity*                    weapon_entity,
                                           i32                        usage_type,
                                           cpu_combat_inode*          cmbt);

        virtual bool check_for_block(      entity*                    target,
                                           i32                        chain_size,
                                     const combo_move_data*           combo_move,
                                           hit_react_record*          react,
                                           weapon_interface_extender* weapon,
                                           entity*                    weapon_entity,
                                           i32                        usage_type,
                                           cpu_combat_inode*          cmbt);

        bool is_a_target(arch_base_vhandle targ) const;

        static bool is_entity_dodging_or_countering(entity_base* target,
                                                    entity_base* attacker,
                                                    bool*        dodging    = nullptr,
                                                    bool*        countering = nullptr);

    protected:
        void start_combat_script(f32 delta_t); // the move's script object, kept or bound anew
        void kill_combat_script(bool unload_exec);

        void push_target(arch_base_vhandle targ);
        i32  internal_auto_calc_targets(const combo_move_data* combo_move, vc8::vector<auto_calc_target>* targets); // how many it found

        void set_delay_stun_recovery(const combo_move_data* combo_move);
        void perform_auto_draw      (const combo_move_data* combo_move);
        void perform_auto_holster   (const combo_move_data* combo_move);

        void update_camera(      f32               delta_t,
                           const combo_move_data*  combo_move,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt);

        void advance_combat_script(      f32               delta_t,
                                   const combo_move_data*  combo_move,
                                         als_inode*        my_als,
                                         cpu_combat_inode* cmbt);

        void apply_hit_dilation_preset(i32 preset, i32 mode); // mode as time_dilation_mode

    private:
        static void wait_for_target_list_lock(); // returns once no other thread holds the lock

        static void add_combat_thread_info   (combat_thread_info* info);
        static void remove_combat_thread_info(combat_thread_info* info); // frees it

        void spawn_attack_sound(      f32               delta_t,
                                const combo_move_data*  combo_move,
                                      als_inode*        my_als,
                                      cpu_combat_inode* cmbt);

        void share_attack_flags_with_hero(      f32               delta_t,
                                          const combo_move_data*  combo_move,
                                                als_inode*        my_als,
                                                cpu_combat_inode* cmbt);

        void probe_wall_ahead(      f32               delta_t,
                              const combo_move_data*  combo_move,
                                    als_inode*        my_als,
                                    cpu_combat_inode* cmbt);

        void update_v_look         (f32 delta_t);
        void update_time_dilation  (als_inode* my_als);
        void auto_calc_throw_target(const combo_move_data* combo_move, als_inode* my_als);

        resource_key      get_sync_react_asg(entity* target) const;     // "sync_controlled_combat_react" for the hero, else "sync_combat_react"
        void              release_sync_target(actor* target);
        arch_base_vhandle engage_sync_target(arch_base_vhandle target); // null when it couldn't be engaged
        bool              engage_sync_targets();                        // whether any target is engaged

        void update_initially_standing();
        bool is_floor_within_reach(); // the floor below is no further down than the floor offset and half a meter
    };

    ASSERT_OFFSETOF(combo_move_data, flags_1, 0x04);
    ASSERT_OFFSETOF(combo_move_data, flags_2, 0x08);
    ASSERT_OFFSETOF(combo_move_data, unk_10,  0x10);
    ASSERT_OFFSETOF(combo_move_data, unk_18,  0x18);
    ASSERT_OFFSETOF(combo_move_data, params,  0x20);

    ASSERT_SIZEOF(auto_calc_target, 0x10);

    ASSERT_SIZEOF  (combat_thread_info,                             0x20);
    ASSERT_OFFSETOF(combat_thread_info, frame_advancing_combo_move, 0x0C);
    ASSERT_OFFSETOF(combat_thread_info, frame_advancing_usage_type, 0x1C);

    ASSERT_SIZEOF  (combat_state,                                    0x2F8);
    ASSERT_OFFSETOF(combat_state, current_buddies,                   0x044);
    ASSERT_OFFSETOF(combat_state, current_targets,                   0x048);
    ASSERT_OFFSETOF(combat_state, combat_anim_values,                0x050);
    ASSERT_OFFSETOF(combat_state, throw_target,                      0x058);
    ASSERT_OFFSETOF(combat_state, combat_script_exec_prefix,         0x05C);
    ASSERT_OFFSETOF(combat_state, combat_script_inst,                0x064);
    ASSERT_OFFSETOF(combat_state, init_move_func_id,                 0x068);
    ASSERT_OFFSETOF(combat_state, spawn_attack_window_sound_func_id, 0x094);
    ASSERT_OFFSETOF(combat_state, unk_098,                           0x098);
    ASSERT_OFFSETOF(combat_state, get_als_facing_func_id,            0x0A8);
    ASSERT_OFFSETOF(combat_state, move_transition_func_id,           0x0CC);
    ASSERT_OFFSETOF(combat_state, get_auto_calc_target_pos_func_id,  0x0E0);
    ASSERT_OFFSETOF(combat_state, update_target_lists_func_id,       0x0F4);
    ASSERT_OFFSETOF(combat_state, preferred_exit_status,             0x0F8);
    ASSERT_OFFSETOF(combat_state, fake_attack_face_dir,              0x100);
    ASSERT_OFFSETOF(combat_state, fake_attack_pos,                   0x10C);
    ASSERT_OFFSETOF(combat_state, chain_size,                        0x13C);
    ASSERT_OFFSETOF(combat_state, signal_flags,                      0x144);
    ASSERT_OFFSETOF(combat_state, callback_ids,                      0x148);
    ASSERT_OFFSETOF(combat_state, strength_test,                     0x198);
    ASSERT_OFFSETOF(combat_state, strength_test_delay,               0x19C);
    ASSERT_OFFSETOF(combat_state, sync_targets,                      0x1A0);
    ASSERT_OFFSETOF(combat_state, sync_buddy,                        0x1A4);
    ASSERT_OFFSETOF(combat_state, next_weapon_index,                 0x1A8);
    ASSERT_OFFSETOF(combat_state, delay_standing,                    0x1AC);
    ASSERT_OFFSETOF(combat_state, target_exit_speed,                 0x1B0);
    ASSERT_OFFSETOF(combat_state, jump_exit_speed,                   0x1C0);
    ASSERT_OFFSETOF(combat_state, initially_standing,                0x1D6);
    ASSERT_OFFSETOF(combat_state, in_attack_window,                  0x1D7);
    ASSERT_OFFSETOF(combat_state, attack_dodged,                     0x1DA);
    ASSERT_OFFSETOF(combat_state, attack_blocked,                    0x1DC);
    ASSERT_OFFSETOF(combat_state, jump_connect_applied,              0x1E1);
    ASSERT_OFFSETOF(combat_state, jump_launch_applied,               0x1E2);
    ASSERT_OFFSETOF(combat_state, delaying_stun,                     0x1E3);
    ASSERT_OFFSETOF(combat_state, in_custom_cam,                     0x1E4);
    ASSERT_OFFSETOF(combat_state, auto_draw_move,                    0x1E8);
    ASSERT_OFFSETOF(combat_state, in_sync_combo_anim,                0x1EA);
    ASSERT_OFFSETOF(combat_state, in_buddy_combo_anim,               0x1EB);
    ASSERT_OFFSETOF(combat_state, attack_sound_spawned,              0x1EC);
    ASSERT_OFFSETOF(combat_state, hit_dilation_waits_for_signal,     0x1F0);
    ASSERT_OFFSETOF(combat_state, hit_dilation_pending,              0x1F2);
    ASSERT_OFFSETOF(combat_state, v_look,                            0x1F4);
    ASSERT_OFFSETOF(combat_state, time_dilation_start_time,          0x210);
    ASSERT_OFFSETOF(combat_state, time_dilation_scale,               0x220);
    ASSERT_OFFSETOF(combat_state, time_dilation_mode,                0x224);
    ASSERT_OFFSETOF(combat_state, hit_sphere_pos,                    0x228);
    ASSERT_OFFSETOF(combat_state, hit_sphere_prev_pos,               0x288);
    ASSERT_OFFSETOF(combat_state, hit_sphere_prev_valid,             0x2E8);
    ASSERT_OFFSETOF(combat_state, attack_end_pending,                0x2F0);
    ASSERT_OFFSETOF(combat_state, weapon_to_use,                     0x2F4);

    // combo_move_data's params
    inline const string_hash script_exe_key                 = string_hash(hash::djb2("script_exe"));
    inline const string_hash script_obj_key                 = string_hash(hash::djb2("script_obj"));
    inline const string_hash script_move_id_key             = string_hash(hash::djb2("script_move_id"));
    inline const string_hash max_targets_key                = string_hash(hash::djb2("max_targets"));
    inline const string_hash jump_launch_force_key          = string_hash(hash::djb2("jump_launch_force"));
    inline const string_hash strength_test_start_value_key  = string_hash(hash::djb2("strength_test_start_value"));
    inline const string_hash combo_move_type_id_key         = string_hash(hash::djb2("combo_move_type_id"));
    inline const string_hash combo_move_name_key            = string_hash(hash::djb2("combo_move_name"));
    inline const string_hash combo_counter_value_key        = string_hash(hash::djb2("combo_counter_value"));
    inline const string_hash combo_counter_delay_key        = string_hash(hash::djb2("combo_counter_delay"));
    inline const string_hash dodge_anim_key                 = string_hash(hash::djb2("dodge_anim"));
    inline const string_hash dodge_type_key                 = string_hash(hash::djb2("dodge_type"));
    inline const string_hash damage_arc_key                 = string_hash(hash::djb2("damage_arc"));
    inline const string_hash damage_radius_key              = string_hash(hash::djb2("damage_radius"));
    inline const string_hash connect_range_mod_key          = string_hash(hash::djb2("connect_range_mod"));
    inline const string_hash attack_time_key                = string_hash(hash::djb2("attack_time"));
    inline const string_hash wounded_category_key           = string_hash(hash::djb2("wounded_category"));
    inline const string_hash attack_sound_group_key         = string_hash(hash::djb2("attack_sound_group"));
    inline const string_hash missed_sound_group_key         = string_hash(hash::djb2("missed_sound_group"));
    inline const string_hash dodged_sound_group_key         = string_hash(hash::djb2("dodged_sound_group"));
    inline const string_hash self_cancel_velocity_dir_key   = string_hash(hash::djb2("self_cancel_velocity_dir"));
    inline const string_hash target_cancel_velocity_dir_key = string_hash(hash::djb2("target_cancel_velocity_dir"));
    inline const string_hash auto_calc_targets_dir_key      = string_hash(hash::djb2("auto_calc_targets_dir"));

    // the defaults of the hash params above
    inline const string_hash wounded_key = string_hash(hash::djb2("Wounded"));
    inline const string_hash attack_key  = string_hash(hash::djb2("attack"));
    inline const string_hash miss_key    = string_hash(hash::djb2("miss"));
    inline const string_hash dodge_key   = string_hash(hash::djb2("dodge"));

    inline const string_hash target_sync_layer_key               = string_hash(hash::djb2("target_sync_layer"));
    inline const string_hash use_multiple_sync_targets_key       = string_hash(hash::djb2("use_multiple_sync_targets"));
    inline const string_hash maintain_previous_throw_target_key  = string_hash(hash::djb2("maintain_previous_throw_target"));
    inline const string_hash auto_calc_throw_target_key          = string_hash(hash::djb2("auto_calc_throw_target"));
    inline const string_hash auto_calc_throw_target_on_start_key = string_hash(hash::djb2("auto_calc_throw_target_on_start"));
    inline const string_hash auto_calc_throw_target_radius_key   = string_hash(hash::djb2("auto_calc_throw_target_radius"));
    inline const string_hash auto_calc_throw_target_arc_key      = string_hash(hash::djb2("auto_calc_throw_target_arc"));

    inline const string_hash meta_type_v_look_key            = string_hash(hash::djb2("meta_type_v_look"));
    inline const string_hash meta_type_v_look_rate_delta_key = string_hash(hash::djb2("meta_type_v_look_rate_delta"));
    inline const string_hash meta_type_v_look_rate_damp_key  = string_hash(hash::djb2("meta_type_v_look_rate_damp"));
    inline const string_hash meta_type_v_look_rate_mute_key  = string_hash(hash::djb2("meta_type_v_look_rate_mute"));

    inline const string_hash hit_effect_key = string_hash(hash::djb2("hit_effect"));

    // spawn_sound_impact's rtpcs and events
    inline const string_hash combo_count_key = string_hash(hash::djb2("combo_count"));
    inline const string_hash damage_key      = string_hash(hash::djb2("damage"));
    inline const string_hash impact_hero_key = string_hash(hash::djb2("impact_hero"));
    inline const string_hash impact_key      = string_hash(hash::djb2("impact"));
    inline const string_hash block_key       = string_hash(hash::djb2("block"));

    namespace references {
        // SM3's combat_state::combat_sync_react_asg, "sync_combat_react"
        inline util::memory_reference<resource_key> combat_sync_react_asg { 0x00FC204C };

        // held around every combat state's current_targets
        inline util::memory_reference<engine_recursive_lock> target_list_lock { 0x00FC23E0 };

        // the running frame_advance threads, made by the first add and deleted with the last entry
        inline util::memory_reference<vc8::list<combat_thread_info*>*> combat_thread_infos     { 0x00FC1EA0 };
        inline util::memory_reference<engine_recursive_lock>           combat_thread_info_lock { 0x00FC2058 };

        // raised on both sides of a sync anim or buddy combo
        inline util::memory_reference<string_hash> ai_combat_sync_begin { 0x0102CB9C };
        inline util::memory_reference<string_hash> ai_combat_sync_end   { 0x0102C674 };

        // the hit time dilation signals on the actor, its targets and the world
        inline util::memory_reference<string_hash> hit_dilation        { 0x0102C5DC };
        inline util::memory_reference<string_hash> hit_dilation_target { 0x0102C61C };
        inline util::memory_reference<string_hash> hit_dilation_world  { 0x0102C400 };

        // raised on a combat state's actor while its strength test runs
        inline util::memory_reference<string_hash> strength_test_finisher_in_range     { 0x0102C4D8 };
        inline util::memory_reference<string_hash> strength_test_finisher_out_of_range { 0x0102C77C };
        inline util::memory_reference<string_hash> strength_test_success               { 0x0102C70C };
        inline util::memory_reference<string_hash> strength_test_failure               { 0x0102CB98 };

        // a strength test's own flag; a success clears it, a failure only while it's set
        inline util::memory_reference<bool> unk_010fa2a4 { 0x010FA2A4 };
    } // references
} // treyarch
