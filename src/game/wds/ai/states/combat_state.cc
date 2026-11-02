#include <cmath>
#include <new>

#include "retail.hh"
#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_rappel_inode.hh"
#include "game/wds/ai/ai_target.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/wds/ai/cpu_combat_inode.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/gun_weapon_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/script_data_interface.hh"
#include "game/wds/entity/interface/spiderman_rvb_player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/als/als.hh"
#include "game/als/internal.hh"
#include "game/als/mocomp.hh"
#include "game/als/state_machine.hh"
#include "game/event/anim_event.hh"
#include "game/event/default_callbacks.hh"
#include "game/animation/animation_controller.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "game/script/script_access.hh"
#include "game/colgeom.hh"
#include "game/app.hh"
#include "game/line_info.hh"
#include "game/local_collision.hh"
#include "game/quest_manager.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"
#include "treyarch/math/trig.hh"

using namespace treyarch;

// ==== combo_move_data ==== //

// sub_4098A0
// inlined @ sub_411870
const char* combo_move_data::get_script_exe(bool* exists) const {
    return params.get_optional_pb_fixedstring(script_exe_key, "", exists);
}

// sub_4098D0
const char* combo_move_data::get_script_obj(bool* exists) const {
    return params.get_optional_pb_fixedstring(script_obj_key, "", exists);
}

// sub_409140
// inlined @ sub_410DB0
f32 combo_move_data::get_script_move_id(bool* exists) const {
    return params.get_optional_pb_float(script_move_id_key, -1.0f, exists);
}

// sub_409020
// inlined @ sub_57B440
// inlined @ sub_4F7470
i32 combo_move_data::get_max_targets(bool* exists) const {
    return params.get_optional_pb_int(max_targets_key, 1, exists);
}

// sub_409050
f32 combo_move_data::get_jump_launch_force(bool* exists) const {
    return params.get_optional_pb_float(jump_launch_force_key, 0.0f, exists);
}

// sub_409180
f32 combo_move_data::get_strength_test_start_value(bool* exists) const {
    return params.get_optional_pb_float(strength_test_start_value_key, 0.1f, exists);
}

als::category_id combo_move_data::get_target_sync_category(bool* exists) const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const combo_move_data*, als::category_id*, bool*))retail::sub_409600)(this, &category, exists);

    return category;
}

als::category_id combo_move_data::get_buddy_sync_category(bool* exists) const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const combo_move_data*, als::category_id*, bool*))retail::sub_409650)(this, &category, exists);

    return category;
}

// sub_548530
// inlined @ sub_54CF20
i32 combo_move_data::get_combo_move_type_id(bool* exists) const {
    return params.get_optional_pb_int(combo_move_type_id_key, -1, exists);
}

// sub_548890
string_hash combo_move_data::get_combo_move_name(bool* exists) const {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    return params.get_optional_pb_hash(combo_move_name_key, none, exists);
}

// sub_4025F0
f32 combo_move_data::get_combo_counter_value(bool* exists) const {
    return params.get_optional_pb_float(combo_counter_value_key, 1.0f, exists);
}

// sub_402620
f32 combo_move_data::get_combo_counter_delay(bool* exists) const {
    return params.get_optional_pb_float(combo_counter_delay_key, -1.0f, exists);
}

// sub_4E2140
const char* combo_move_data::get_dodge_anim(bool* exists) const {
    return params.get_optional_pb_fixedstring(dodge_anim_key, "", exists);
}

// sub_4E20E0
i32 combo_move_data::get_dodge_type(bool* exists) const {
    return params.get_optional_pb_int(dodge_type_key, -1, exists);
}

// sub_4090C0
f32 combo_move_data::get_damage_arc(bool* exists) const {
    return params.get_optional_pb_float(damage_arc_key, 90.0f, exists);
}

// sub_409100
f32 combo_move_data::get_attack_time(bool* exists) const {
    return params.get_optional_pb_float(attack_time_key, -1.0f, exists);
}

// sub_402750
string_hash combo_move_data::get_wounded_category(bool* exists) const {
    string_hash wounded;
    wounded.initialize(mash::ALLOCATED, nullptr, wounded_key.source_hash_code);

    return params.get_optional_pb_hash(wounded_category_key, wounded, exists);
}

// sub_409760
string_hash combo_move_data::get_attack_sound_group(bool* exists) const {
    string_hash attack;
    attack.initialize(mash::ALLOCATED, nullptr, attack_key.source_hash_code);

    return params.get_optional_pb_hash(attack_sound_group_key, attack, exists);
}

// sub_4097B0
string_hash combo_move_data::get_missed_sound_group(bool* exists) const {
    string_hash miss;
    miss.initialize(mash::ALLOCATED, nullptr, miss_key.source_hash_code);

    return params.get_optional_pb_hash(missed_sound_group_key, miss, exists);
}

// sub_4027F0
string_hash combo_move_data::get_dodged_sound_group(bool* exists) const {
    string_hash dodge;
    dodge.initialize(mash::ALLOCATED, nullptr, dodge_key.source_hash_code);

    return params.get_optional_pb_hash(dodged_sound_group_key, dodge, exists);
}

// sub_409380
vector3d combo_move_data::get_self_cancel_velocity_dir(bool* exists) const {
    return params.get_optional_pb_vector3d(self_cancel_velocity_dir_key, vector3d(0.0f, 0.0f, 0.0f), exists);
}

// sub_4093D0
vector3d combo_move_data::get_target_cancel_velocity_dir(bool* exists) const {
    return params.get_optional_pb_vector3d(target_cancel_velocity_dir_key, vector3d(0.0f, 0.0f, 0.0f), exists);
}

// sub_409420
vector3d combo_move_data::get_auto_calc_targets_dir(bool* exists) const {
    return params.get_optional_pb_vector3d(auto_calc_targets_dir_key, vector3d(0.0f, 0.0f, 1.0f), exists);
}

// ==== combat moves ==== //

// sub_401300
f32 treyarch::get_combat_move_script_code(e_combat_move move, i32 script_move) {
    switch (move) {
        case combat_move_light_melee:        return 0.0f;
        case combat_move_heavy_melee:        return 1.0f;
        case combat_move_special_attack:     return 2.0f;
        case combat_move_jump_attack:        return 3.0f;
        case combat_move_bounce_attack:      return 4.0f;
        case combat_move_counter_attack:     return 7.0f;
        case combat_move_dodge:              return 20.0f;
        case combat_move_block:              return 21.0f;
        case combat_move_save_civ:           return 11.0f;
        case combat_move_light_ranged:       return 5.0f;
        case combat_move_heavy_ranged:       return 6.0f;
        case combat_move_grapple:            return 8.0f;
        case combat_move_throw_manip:        return 9.0f;
        case combat_move_grab_civ:           return 10.0f;
        case combat_move_boss_finisher:      return 12.0f;
        case combat_move_car_finisher:       return 13.0f;
        case combat_move_boost_power_attack: return 14.0f;
        case combat_move_boost_speed_attack: return 15.0f;
        case combat_move_boost_air_attack:   return 16.0f;
        case combat_move_boost_web_attack:   return 17.0f;
        case combat_move_manual_dodge:       return 18.0f;
        case combat_move_script_move:        return (f32)((f64)(f32)script_move + 22.0);
        default:                             return -1.0f;
    }
}

// ==== combat_state ==== //

const vector3d combat_state::unset_fire_position(-10000.0f, -10000.0f, -10000.0f);

// slot 1
void combat_state::construct_mashed_class() {
    ((void (__thiscall*)(combat_state*))retail::sub_4122B0)(this);
}

// slot 2
void combat_state::destruct_mashed_class() {
    ((void (__thiscall*)(combat_state*))retail::sub_412310)(this);
}

// slot 3
void combat_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(combat_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_417CE0)(this,
                                                                                                               mash_info,
                                                                                                               containing_class_ptr,
                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key combat_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const combat_state*))retail::sub_4D07B0)(this);
}

// slot 6
bool combat_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const combat_state*, mash::virtual_types_key))retail::sub_5808A0)(this, parent_class);
}

// sub_4D07C0
i32 combat_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void combat_state::activate(      ai_state_machine* the_state_machine,
                            const mashed_state*     the_mashed_state,
                            const mashed_state*     prev_mashed_state,
                            const param_block*      extra_params) {

    ((void (__thiscall*)(combat_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_41A8A0)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void combat_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(combat_state*, const mashed_state*))retail::sub_418F90)(this, next_mashed_state);
}

// sub_41F860
e_state_exit_code combat_state::frame_advance(f32 delta_t) {
    if (in_custom_cam)
        retail::sub_77D3A0(300, 200, (i32)0xFFFF0000, (char*)"Custom Cam!!!"); // a screen print

    e_state_exit_code the_exit_code = enhanced_state::frame_advance(delta_t);

    auto*         my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);
    auto*         cmbt   = (cpu_combat_inode*)get_core()->get_info_node(info_node_type_cpu_combat);
    als::layer_id layer  = get_combat_layer();

    const combo_move_data* combo_move = my_als->get_combo_move(layer);

    // the chain starts with the state, unless the layer is already in the middle of a combat move
    if (!chain_size) {
        string_hash combat_category = get_combat_als_category();

        if (my_als->get_category_id(layer) == combat_category && combo_move)
            chain_size = 1;

        if (!chain_size) {
            signal_flags = 0;

            i32           next_script_move_num = -1;
            e_combat_move initial_move         = get_initial_move(&next_script_move_num);

            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_combat_desired_move,         (f32)initial_move);
            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_combat_script_desired_move,  (f32)next_script_move_num);
            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_combat_strength_test_status, -2.0f);
            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_desired_up_vector,           get_actor()->my_abs_po->get_y_facing());

            set_als_position(my_als, nullptr);
            set_als_facing(my_als, nullptr);
            unk_026(my_als, nullptr);
            add_extra_als_params(my_als, nullptr);

            my_als->request_category_transition(get_combat_als_category(), layer, false, false, true, false);
            my_als->my_als->force_update(0.0001f, true);

            if (get_actor()->has_ifc(entity_ifc_standing) && get_actor()->standing_ifc()->is_enabled())
                get_actor()->standing_ifc()->manage_standing(0.01f, get_actor()->standing_ifc()->is_effectively_standing(), -1.0f);
        }
    }

    combo_move = my_als->get_combo_move(layer);

    // a move that can't be interrupted holds off every other exit, unless the combo breaker is up
    bool chain_runs = the_exit_code == state_exit_code_no_exit;

    if (!chain_runs                                                                                          &&
        combo_move                                                                                           &&
        (combo_move->flags_1 & combo_flag_non_interruptable || combo_move->flags_1 & combo_flag_unbreakable) &&
        !(get_actor()->has_ifc(entity_ifc_damage) && get_actor()->damage_ifc()->flags & damage_ifc_flag_combat_combo_breaker)) {

        switch (the_exit_code) {
            case state_exit_code_death:
            case state_exit_code_biped_physics:
            case state_exit_code_dodge:
            case state_exit_code_block:
            case state_exit_code_swim:
                break;

            default:
                chain_runs = true;
        }
    }

    // landing, or the ground splat when the fall hurts
    auto land = [&]() -> e_state_exit_code {
        if (get_actor()->has_physical_ifc()) {
            vector3d velocity = get_actor()->physical_ifc()->unk_020;

            if (unk_031(-0.0f - velocity.y) && can_handle_exit_code(state_exit_code_ground_splat))
                return state_exit_code_ground_splat;
        }

        return state_exit_code_land;
    };

    if (chain_runs) {
        bool splat = false;

        if (get_actor()->has_ifc(entity_ifc_physical)) {
            u32 physical_flags = get_actor()->physical_ifc()->flags;

            if (physical_flags & physical_interface_flag_collided || physical_flags & physical_interface_flag_ext_collided) {
                vector3d normal = get_actor()->physical_ifc()->last_collision_normal;

                if (normal.y > 0.7f && (!combo_move || combo_move->unk_10 & combo_unk_10_flag_00000040)) {
                    vector3d velocity = get_actor()->physical_ifc()->unk_020;

                    splat = unk_031(-velocity.y) && can_handle_exit_code(state_exit_code_ground_splat);
                }
            }
        }

        if (combo_move                                         &&
            combo_move->flags_2 & combo_flag_maintain_standing &&
            get_actor()->has_ifc(entity_ifc_standing)          &&
            get_actor()->standing_ifc()->is_enabled())

            get_actor()->standing_ifc()->manage_standing(true, 5.0f);

        bool standing;

        if (get_actor()->has_ifc(entity_ifc_standing) &&
            get_actor()->standing_ifc()->is_enabled() &&
            get_actor()->standing_ifc()->is_effectively_standing())

            standing = true;
        else {
            standing = false;

            if (get_actor()->has_ifc(entity_ifc_standing) && get_actor()->standing_ifc()->is_enabled() && is_floor_within_reach())
                standing = true;
            else if (initially_standing && jump_connect_applied)
                initially_standing = false;
        }

        auto end_chain = [&]() -> e_state_exit_code {
            arch_base_vhandle first_target = current_targets->empty() ? arch_base_vhandle() : (*current_targets)[0];

            kill_strength_test();
            kill_sync_anim(false);
            kill_buddy_combo(false);

            if (target_exit_speed > 0.0001f)
                unk_059(first_target);

            if (get_actor()->has_ifc(entity_ifc_standing)) {
                if (jump_exit_speed > 0.0001f)
                    unk_058();

                if (delay_standing > 0.0f) {
                    retail::sub_5FF3B0((i32)get_actor()->standing_ifc(), delay_standing); // standing_interface::unk_048 and its flag
                    get_actor()->standing_ifc()->clear_effectively_standing();

                    standing = false;
                } else {
                    get_actor()->standing_ifc()->manage_standing(0.01f, true, 0.15f);

                    if (get_actor()->has_standing_ifc() && get_actor()->standing_ifc()->is_enabled() && get_actor()->standing_ifc()->is_effectively_standing())
                        standing = true;
                    else {
                        standing = false;

                        if (get_actor()->has_standing_ifc() && get_actor()->standing_ifc()->is_enabled() && is_floor_within_reach())
                            standing = true;
                    }
                }
            }

            if (chain_size <= 0)
                return state_exit_code_failure;

            if (preferred_exit_status != state_exit_code_no_exit) {
                get_actor();

                return preferred_exit_status;
            }

            auto* rappel = (ai_rappel_inode*)get_core()->get_info_node(info_node_type_ai_rappel);

            if (rappel && rappel->is_rappel_requested())
                return can_handle_exit_code(state_exit_code_rappel) ? state_exit_code_rappel : state_exit_code_success;

            if (splat)
                return state_exit_code_ground_splat;

            if (!get_actor()->has_standing_ifc() || !get_actor()->standing_ifc()->is_enabled())
                return state_exit_code_success;

            if (!standing) {
                if (can_handle_exit_code(state_exit_code_jump))
                    return state_exit_code_jump;

                if (can_handle_exit_code(state_exit_code_fall))
                    return state_exit_code_fall;

                return state_exit_code_success;
            }

            if (can_handle_exit_code(state_exit_code_land))
                return land();

            if (can_handle_exit_code(state_exit_code_stand))
                return state_exit_code_stand;

            return state_exit_code_success;
        };

        auto run_chain = [&]() -> e_state_exit_code {
            if (splat)
                return end_chain();

            // the actor left the ground or found it while the move didn't expect it
            if (!in_sync_combo_anim                                                       &&
                !my_als->my_als->get_als_layer(als::base_layer)->did_transition_succeed() &&
                !(combo_move && combo_move->flags_1 & combo_flag_ignore_standing)         &&
                standing != (bool)initially_standing) {

                if (preferred_exit_status != state_exit_code_no_exit)
                    return preferred_exit_status;

                if (!standing) {
                    if (can_handle_exit_code(state_exit_code_fall))
                        return state_exit_code_fall;

                    return chain_size <= 0 ? state_exit_code_failure : state_exit_code_success;
                }

                if (can_handle_exit_code(state_exit_code_land))
                    return land();

                if (can_handle_exit_code(state_exit_code_stand))
                    return state_exit_code_stand;

                return chain_size <= 0 ? state_exit_code_failure : state_exit_code_success;
            }

            if (is_combo_cancelled(combo_move, my_als, delta_t))
                return end_chain();

            string_hash combat_category = get_combat_als_category();

            if (!(my_als->get_category_id(layer) == combat_category)) {
                if (!my_als->anim_finished(get_combat_als_category(), layer, false, true))
                    return state_exit_code_no_exit;

                return end_chain();
            }

            bool has_block_exit = false;
            f32  block_exit     = my_als->get_time_to_signal(als::references::block_exit_switch_time.read(), als::base_layer, &has_block_exit, true, nullptr);

            if (!has_block_exit) {
                if (get_actor()->has_ifc(entity_ifc_player) && get_actor()->player_ifc()->unk_265())
                    return state_exit_code_block;
            } else if (!(0.0f < block_exit) && get_actor()->has_player_ifc() && get_actor()->player_ifc()->unk_265())
                return state_exit_code_block;

            auto past_early_exit = [&]() -> bool { // the base layer is past its early exit switch time
                return ((bool (__thiscall*)(als_inode*, als::layer_id))retail::sub_44F2D0)(my_als, als::base_layer);
            };

            if (past_early_exit()                           &&
                can_handle_exit_code(state_exit_code_swing) &&
                get_actor()->has_player_ifc()               &&
                get_actor()->player_ifc()->unk_390())

                return state_exit_code_swing;

            if (past_early_exit()                           &&
                get_actor()->has_ifc(entity_ifc_player)     &&
                get_actor()->player_ifc()->handle_jump_charge(delta_t) &&
                can_handle_exit_code(state_exit_code_jump_launch))

                return state_exit_code_jump_launch;

            if (past_early_exit()                                                              &&
                get_actor()->has_ifc(entity_ifc_player)                                        &&
                get_actor()->player_ifc()->get_command_button(plr_cmd_prop_throw)->triggered_or_missed_press() &&
                get_actor()->player_ifc()->unk_520())

                return state_exit_code_prop_throw;

            e_combat_move next_move            = combat_num_moves;
            i32           next_script_move_num = -1;

            if ((combo_move && combo_move->unk_10 & combo_unk_10_flag_00000010) ||
                (get_actor()->has_ifc(entity_ifc_standing) && get_actor()->standing_ifc()->flags & standing_interface_flag_adv_standing))

                my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_desired_up_vector, get_actor()->my_abs_po->get_y_facing());
            else
                my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_desired_up_vector, references::unk_00e6049c.get());

            bool transitioned = my_als->my_als->get_als_layer(layer)->did_transition_succeed();
            bool by_request   = my_als->my_als->get_als_layer(layer)->did_transition_succeed_from_request();

            // a new move: settle the last one, then set this one up
            if (transitioned) {
                my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_button_held_through_state, 1.0f);

                get_actor()->get_ai_core()->flags &= ~ai_core_flag_attack_connected;

                if (combo_move && combo_move->unk_18 & combo_unk_18_flag_00008000)
                    hit_dilation_pending = false;

                arch_base_vhandle prev_first_target = current_targets->empty() ? arch_base_vhandle() : (*current_targets)[0];

                if (combo_move && combo_move->flags_3 & combo_flag_3_unk_00020000) {
                    initially_standing = get_actor()->has_standing_ifc()           &&
                                         get_actor()->standing_ifc()->is_enabled() &&
                                         get_actor()->standing_ifc()->is_effectively_standing();

                    if (!initially_standing && get_actor()->has_standing_ifc() && get_actor()->standing_ifc()->is_enabled() && is_floor_within_reach())
                        initially_standing = true;
                }

                if (in_attack_window)
                    exit_attack_window(combo_move, my_als, cmbt);

                perform_auto_holster(combo_move);
                kill_strength_test();

                kill_sync_anim(combo_move                                             &&
                               combo_move->flags_1 & combo_flag_maintain_prev_targets &&
                               (combo_move->flags_2 & combo_flag_sync_anim || combo_move->flags_2 & combo_flag_strength_test));

                kill_buddy_combo(combo_move && combo_move->flags_2 & combo_flag_buddy_combo);

                if (jump_exit_speed > 0.0001f)
                    unk_058();

                if (target_exit_speed > 0.0001f)
                    unk_059(prev_first_target);

                if (delay_standing > 0.0f) {
                    retail::sub_5FF3B0((i32)get_actor()->standing_ifc(), delay_standing); // standing_interface::unk_048 and its flag
                    get_actor()->standing_ifc()->clear_effectively_standing();
                }

                if (!((bool (__thiscall*)(als::state_machine_interface*))retail::sub_40A850)(my_als->my_als->get_als_layer(layer))) {
                    time_dilation_scale       = 1.0f;
                    time_dilation_start_scale = 1.0f;
                    time_dilation_start_time  = F32_MAX;
                    time_dilation_end_scale   = 1.0f;
                    time_dilation_end_time    = F32_MAX;

                    update_time_dilation(my_als);
                }

                if (by_request) {
                    if (!combo_move || !(combo_move->flags_1 & combo_flag_maintain_prev_targets))
                        clear_targets();

                    if (!my_als->get_optional_als_pb_int(maintain_previous_throw_target_key, 0, als::base_layer, nullptr))
                        throw_target = arch_base_vhandle();

                    update_next_move();

                    retail::sub_7EAA00((u8*)quest_manager::inst()); // quest_manager::unk_037 set
                }

                if (current_targets->empty() ||
                    (combo_move && (combo_move->flags_3 & combo_flag_3_unk_00800000 || combo_move->flags_3 & combo_flag_3_unk_04000000))) {

                    clear_targets();

                    vc8::vector<ai_target> auto_calc_targets;

                    if (internal_auto_calc_targets(combo_move, &auto_calc_targets) > 0) {
                        i32 max_targets = 1;

                        if (combo_move)
                            max_targets = combo_move->get_max_targets(nullptr);

                        for (auto *target = auto_calc_targets.begin(), *end = auto_calc_targets.end(); target != end; ++target) {
                            if (current_targets->size() >= max_targets)
                                break;

                            push_target(target->target);
                        }
                    }
                }

                if (my_als->get_optional_als_pb_int(auto_calc_throw_target_on_start_key, 0, als::base_layer, nullptr))
                    auto_calc_throw_target(combo_move, my_als);

                if (by_request) {
                    arch_base_vhandle first_target = current_targets->empty() ? arch_base_vhandle() : (*current_targets)[0];

                    if (!first_target.null() && first_target == prev_first_target)
                        unk_140++;
                    else
                        unk_140 = 1;
                }

                start_combat_script(delta_t);
                move_transition(combo_move, by_request);
                wait_for_target_list_lock();

                if (combo_move) {
                    if (get_actor()->has_combo_ifc() && combo_move->flags_1 & combo_flag_clear_combo_count)
                        retail::sub_626600((u32*)get_actor()->combo_ifc(), 0); // combo_interface: sets the combo count

                    if (get_actor()->has_player_ifc() && combo_move->flags_2 & combo_flag_reset_double_jump)
                        get_actor()->player_ifc()->unk_183(true);
                }

                chuck::vm::chuck_dynamic_array_t* targets = current_targets->array;

                for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element) {
                    auto* target = (entity_base*)((arch_base_vhandle*)element)->resolve();

                    if (!target)
                        continue;

                    if (combo_move                                                     &&
                        !(combo_move->flags_3 & combo_flag_ignore_add_incoming_attack) &&
                        (by_request || combo_move->flags_2 & combo_flag_force_add_incoming_attack))

                        add_incoming_attack(target, get_actor(), combo_move, by_request + chain_size);

                    if (target->is_an_actor() && target->has_ifc(entity_ifc_player))
                        target->player_ifc()->unk_222((i32)get_actor(), (i32)combo_move, by_request + chain_size);

                    if (target->is_an_actor()) {
                        if (combo_move && combo_move->flags_1 & combo_flag_ignore_target_collisions) {
                            if (target->get_colgeom() && target->get_colgeom()->get_type() == collision_geometry_type_capsule)
                                get_actor()->add_collision_ignorance(target->my_handle);
                        } else
                            get_actor()->remove_collision_ignorance(target->my_handle);
                    }

                    if (combo_move && combo_move->flags_2 & combo_flag_cancel_target_stun && target->has_ifc(entity_ifc_damage))
                        retail::sub_6527E0((u32*)target->damage_ifc(), true); // damage_interface: cancels the stun
                }

                // the facing the move fakes, turned by the next target angle about the actor's up
                if (by_request) {
                    fake_attack_face_dir = get_actor()->my_abs_po->get_z_facing();
                    fake_attack_face_dir.normalize();

                    if (((als::state_machine*)my_als->my_als->get_als_layer(als::base_layer))->has_ext_param_been_set(als::param_combat_next_target_angle)) {
                        als::animation_logic_system_interface* the_als = my_als->my_als;

                        f32 angle = (f32)(((als::state_machine*)the_als->get_als_layer(als::base_layer))->get_float_param((als::animation_logic_system*)the_als,
                                                                                                                          als::param_combat_next_target_angle) *
                                          0.017453293005625408);

                        po rotation;
                        rotation.identity();

                        angle = -angle;

                        vector3d up = get_actor()->my_abs_po->get_y_facing();
                        up.normalize();

                        rotation.set_rot(math::vector4(up.x, up.y, up.z, 0.0f), angle);

                        const matrix4x4 &m   = rotation.get_matrix();
                        const vector3d   dir = fake_attack_face_dir;

                        fake_attack_face_dir = vector3d((f32)((f64)m.x.x * dir.x + (f64)m.y.x * dir.y + (f64)m.z.x * dir.z + (f64)m.w.x * 0.0),
                                                        (f32)((f64)m.x.y * dir.x + (f64)m.y.y * dir.y + (f64)m.z.y * dir.z + (f64)m.w.y * 0.0),
                                                        (f32)((f64)m.x.z * dir.x + (f64)m.y.z * dir.y + (f64)m.z.z * dir.z + (f64)m.w.z * 0.0));

                        fake_attack_face_dir.normalize();
                    }

                    const vector3d &position = get_actor()->my_abs_po->get_position();

                    vector3d ahead((f32)((f64)fake_attack_face_dir.x * 1.5),
                                   (f32)((f64)fake_attack_face_dir.y * 1.5),
                                   (f32)((f64)fake_attack_face_dir.z * 1.5));

                    fake_attack_pos = vector3d((f32)((f64)ahead.x + position.x),
                                               (f32)((f64)ahead.y + position.y),
                                               (f32)((f64)ahead.z + position.z));

                    chain_size++;
                }

                set_delay_stun_recovery(combo_move);
                start_strength_test(combo_move, my_als, cmbt, delta_t);
                start_sync_anim(combo_move);
                start_buddy_combo(combo_move);

                attack_sound_spawned = false;

                // the move's camera: its flag, else the script's say
                if (custom_camera_func_id == -1) {
                    if (combo_move && combo_move->flags_3 & combo_flag_animated_camera)
                        signal_flags |= combat_signal_animated_camera_on;
                    else
                        signal_flags |= combat_signal_animated_camera_off;
                } else if (custom_camera_engaged_func_id == -1)
                    signal_flags |= combat_signal_animated_camera_on;
                else if (chuck::vm::script_instance* inst = script::get_instance(combat_script_inst)) {
                    f32 engaged = 0.0f;

                    chuck::vm::vm_thread* thread = script::create_thread(custom_camera_engaged_func_id, inst, get_actor());

                    script::push_value(thread, delta_t);
                    script::push_value(thread, current_targets->array);
                    script::push_value(thread, combat_anim_values->array);
                    script::run_thread_with_return(thread, &engaged);

                    if (!(0.5f > engaged))
                        signal_flags |= combat_signal_animated_camera_on;
                } else
                    signal_flags |= combat_signal_animated_camera_on;
            } else
                next_move = get_next_move(delta_t, &next_script_move_num);

            update_time_dilation(my_als);

            (*combat_anim_values)[cmbt_unk_50] = next_move == combat_num_moves ? -1.0f : get_combat_move_script_code(next_move, next_script_move_num);

            f32 strength_test_status = -2.0f;

            if (strength_test) {
                ((void (__thiscall*)(plr_strength_test*, f32))retail::sub_92C630)(strength_test, delta_t); // plr_strength_test::frame_advance

                strength_test_delay.time = (f32)((f64)strength_test_delay.time - delta_t);

                if (0.0f > strength_test_delay.time)
                    strength_test_delay.time = 0.0f;

                if (strength_test_delay.time > 0.0f) {
                    f32 start_value = combo_move ? combo_move->get_strength_test_start_value(nullptr) : 0.1f;

                    ((void (__thiscall*)(plr_strength_test*, f32, i32))retail::sub_90C680)(strength_test, start_value, 0);
                }

                if (((bool (__thiscall*)(plr_strength_test*))retail::sub_90C630)(strength_test))
                    strength_test_delay.time = 0.0f;

                if (!(combo_move->flags_3 & combo_flag_strength_test_looping_anim)) {
                    animation_controller* ctrl = get_actor()->anim_ctrl;

                    f32 norm_time = ((f32 (__thiscall*)(plr_strength_test*))retail::sub_91CE30)(strength_test);

                    if (norm_time >= 1.0f)
                        norm_time = 0.9999f;

                    ctrl->set_anim_normalized_time(0, norm_time);
                }

                static const string_hash strength_test_force_failure_key = string_hash(hash::djb2("script_data_strength_test_force_failure"));

                bool force_failure = false;

                if (get_actor()->has_ifc(entity_ifc_script_data)) {
                    f32 value = 0.0f;

                    if (get_actor()->script_data_ifc()->get_ifc_num(strength_test_force_failure_key, &value, false))
                        force_failure = value > 0.0001f;
                }

                strength_test_status = -1.0f;

                if (((bool (__thiscall*)(plr_strength_test*))retail::sub_91CF00)(strength_test))
                    strength_test_status = 1.0f;
                else if (force_failure || ((bool (__thiscall*)(plr_strength_test*))retail::sub_90C640)(strength_test))
                    strength_test_status = 0.0f;

                if (((bool (__thiscall*)(plr_strength_test*))retail::sub_90C600)(strength_test)) {
                    bool in_range = ((bool (__thiscall*)(plr_strength_test*))retail::sub_91CE80)(strength_test);

                    get_actor()->raise_event(in_range ? references::strength_test_finisher_in_range.read() : references::strength_test_finisher_out_of_range.read());
                }

                if (cmbt) {
                    f32 amount = ((f32 (__thiscall*)(plr_strength_test*))retail::sub_90C6A0)(strength_test);

                    cmbt->set_script_strength_test_value(amount);
                    cmbt->set_script_strength_test_status(strength_test_status);
                }
            }

            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_strength_test_status, strength_test_status);

            if (strength_test_status == 1.0f) {
                get_actor()->raise_event(references::strength_test_success.read());
                retail::sub_90C790(false);
            } else if (strength_test_status == 0.0f) {
                get_actor()->raise_event(references::strength_test_failure.read());

                if (references::unk_010fa2a4.read())
                    retail::sub_90C790(false);
            }

            if (!jump_launch_applied                            &&
                combo_move                                      &&
                combo_move->flags_3 & combo_flag_3_unk_00000400 &&
                combo_move->unk_18 & combo_unk_18_flag_00000008)

                unk_060(delta_t, combo_move);

            if (signal_flags & combat_signal_anim_action) {
                if (auto_draw_move)
                    perform_auto_draw(combo_move);
                else if (auto_holster_move)
                    perform_auto_holster(combo_move);

                if (!jump_launch_applied                               &&
                    combo_move                                         &&
                    combo_move->flags_3 & combo_flag_3_unk_00000400    &&
                    combo_move->get_jump_launch_force(nullptr) != 0.0f &&
                    get_actor()->has_physical_ifc()) {

                    jump_launch_applied = unk_056(combo_move);
                    initially_standing  = false;
                }
            }

            const combo_move_data* next_combo_move = my_als->get_combo_move(layer);

            fill_combat_anim_values(next_combo_move, my_als, delta_t);

            // between moves the script may pick the next one
            if (!transitioned) {
                chuck::vm::script_instance* inst = script::get_instance(combat_script_inst);

                if (inst && combat_move_func_id != -1) {
                    chuck::vm::vm_thread* thread = script::create_thread(combat_move_func_id, inst, get_actor());

                    script::push_value(thread, get_combat_move_script_code(next_move, next_script_move_num));
                    script::push_value(thread, current_targets->array);
                    script::push_value(thread, combat_anim_values->array);

                    f32 script_move = -1.0f;
                    script::run_thread_with_return(thread, &script_move);

                    next_script_move_num = (i32)script_move;

                    if (next_script_move_num >= 0)
                        next_move = combat_move_script_move;
                }
            }

            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_combat_desired_move,        (f32)next_move);
            my_als->my_als->get_als_layer(layer)->set_desired_param(als::param_combat_script_desired_move, (f32)next_script_move_num);

            if (my_als->get_optional_als_pb_int(auto_calc_throw_target_key,           0, als::base_layer, nullptr) &&
                !my_als->get_optional_als_pb_int(auto_calc_throw_target_on_start_key, 0, als::base_layer, nullptr))

                auto_calc_throw_target(next_combo_move, my_als);

            set_als_position(my_als, next_combo_move);
            set_als_facing(my_als, next_combo_move);
            unk_026(my_als, next_combo_move);
            add_extra_als_params(my_als, next_combo_move);

            if (next_move != combat_num_moves)
                my_als->request_category_transition(get_combat_als_category(), layer, false, false, true, false);

            update_combat(delta_t, next_move, my_als, cmbt);

            return state_exit_code_no_exit;
        };

        the_exit_code = run_chain();
    }

    update_camera(delta_t, combo_move, my_als, cmbt);
    spawn_attack_sound(delta_t, combo_move, my_als, cmbt);
    share_attack_flags_with_hero(delta_t, combo_move, my_als, cmbt);
    probe_wall_ahead(delta_t, combo_move, my_als, cmbt);
    update_v_look(delta_t);

    signal_flags = 0;

    if (get_actor()->has_ifc(entity_ifc_damage))
        get_actor()->damage_ifc()->flags &= ~damage_ifc_flag_combat_combo_breaker;

    if (cmbt)
        cmbt->set_script_chain_size(chain_size);

    return the_exit_code;
}

// sub_54B5C0
void combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    the_list.add_entry(als_inode::my_type_key);
}

// sub_667F80
bool combat_state::is_combat_state() const {
    return true;
}

// sub_8139C0
u32 combat_state::unk_019() const {
    return 2;
}

// sub_A90D90
e_combat_move combat_state::get_initial_move(i32*) {
    return combat_move_light_melee;
}

// slot 23
void combat_state::move_transition(const combo_move_data* new_combo_move, u32 unk) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*, u32))retail::sub_417D50)(this, new_combo_move, unk);
}

// slot 24
void combat_state::add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, als_inode*, const combo_move_data*))retail::sub_413AC0)(this, my_als, combo_move);
}

// slot 25
void combat_state::set_als_facing(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, als_inode*, const combo_move_data*))retail::sub_414870)(this, my_als, combo_move);
}

// sub_415470
void combat_state::unk_026(als_inode* my_als, const combo_move_data* combo_move) {
    if (combo_move                                         &&
        !(combo_move->unk_10 & combo_unk_10_flag_00000200) &&
        !(combo_move->unk_10 & combo_unk_10_flag_00000400) &&
        !(combo_move->unk_10 & combo_unk_10_flag_00100000))

        return;

    vector3d up = get_actor()->my_abs_po->get_y_facing();

    get_actor();

    if (combo_move) {
        if (combo_move->unk_10 & combo_unk_10_flag_00000400)
            up = references::unk_00e6049c.get();
        else if (combo_move->unk_10 & combo_unk_10_flag_00100000) {
            // the first target that isn't dodging or countering
            if (!current_targets->empty())
                for (auto it = current_targets->begin(), end = current_targets->end(); it != end; ++it) {
                    auto* target = (entity_base*)(*it).resolve();

                    if (target && !is_entity_dodging_or_countering(target, get_actor())) {
                        up = target->my_abs_po->get_y_facing();

                        break;
                    }
                }
        } else if (!current_targets->empty()) {
            // the targets' average position, only the first's for a primary target move
            vector3d average = references::unk_00fc1ee0.get();
            f32      count   = 0.0f;

            for (auto it = current_targets->begin(), end = current_targets->end(); it != end; ++it) {
                auto* target = (entity_base*)(*it).resolve();

                if (!target)
                    continue;

                if (!is_entity_dodging_or_countering(target, get_actor())) {
                    const vector3d &position = target->my_abs_po->get_position();

                    average = vector3d((f32)((f64)position.x + average.x),
                                       (f32)((f64)position.y + average.y),
                                       (f32)((f64)position.z + average.z));

                    count = (f32)((f64)count + 1.0);
                }

                if (combo_move->flags_2 & combo_flag_primary_target_movement)
                    break;
            }

            if (count > 0.0001f) {
                if (count > 1.01f) {
                    f32 inv_count = (f32)(1.0 / count);

                    average = vector3d((f32)((f64)inv_count * average.x),
                                       (f32)((f64)inv_count * average.y),
                                       (f32)((f64)inv_count * average.z));
                }

                vector3d toward;

                if (combo_move->flags_2 & combo_flag_primary_target_movement)
                    toward = average;
                else {
                    // the target nearest the average, else the point a meter ahead
                    f32 nearest = F32_MAX;

                    toward = get_actor()->my_abs_po->get_z_facing();

                    const vector3d &position = get_actor()->my_abs_po->get_position();

                    toward = vector3d((f32)((f64)position.x + toward.x),
                                      (f32)((f64)position.y + toward.y),
                                      (f32)((f64)position.z + toward.z));

                    for (auto it = current_targets->begin(), end = current_targets->end(); it != end; ++it) {
                        auto* target = (entity_base*)(*it).resolve();

                        if (!target)
                            continue;

                        const vector3d &target_position = target->my_abs_po->get_position();

                        f32 dx = (f32)((f64)target_position.x - average.x);
                        f32 dy = (f32)((f64)target_position.y - average.y);
                        f32 dz = (f32)((f64)target_position.z - average.z);

                        f32 distance2 = (f32)((f64)dy * dy + (f64)dx * dx + (f64)dz * dz);

                        if (nearest > distance2) {
                            toward  = target_position;
                            nearest = distance2;
                        }
                    }
                }

                const vector3d &position = get_actor()->my_abs_po->get_position();

                vector3d dir((f32)((f64)toward.x - position.x),
                             (f32)((f64)toward.y - position.y),
                             (f32)((f64)toward.z - position.z));

                // the z row of a frame facing +Y with the direction as its normal, unless it lines up with the actor's
                if (dir.length2_float() > 0.0001f) {
                    dir.normalize();

                    const vector3d &origin = get_actor()->my_abs_po->get_position();
                    const vector3d &y_axis = references::unk_00e6049c.get();

                    po frame(math::vector4(y_axis.x, y_axis.y, y_axis.z, 0.0f),
                             math::vector4(dir.x,    dir.y,    dir.z,    0.0f),
                             math::vector4(origin.x, origin.y, origin.z, 0.0f));

                    vector3d frame_z = frame.get_z_facing();

                    f32 along = std::fabs((f32)(frame_z * get_actor()->my_abs_po->get_z_facing()));

                    if (0.99f > along)
                        up = frame_z;
                }
            }
        }

        if (get_als_up_func_id != -1)
            if (chuck::vm::script_instance* inst = script::get_instance(combat_script_inst)) {
                chuck::vm::vm_thread* thread = script::create_thread(get_als_up_func_id, inst, get_actor());

                script::push_value(thread, up);
                script::push_value(thread, combo_move->get_script_move_id(nullptr));
                script::push_value(thread, current_targets->array);
                script::push_value(thread, combat_anim_values->array);
                script::run_thread_with_return(thread, &up);
            }
    }

    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_desired_up_vector, up);
}

// slot 27
void combat_state::set_als_position(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, als_inode*, const combo_move_data*))retail::sub_415B20)(this, my_als, combo_move);
}

// sub_41C930
void combat_state::update_combat(f32               delta_t,
                                 e_combat_move     next_move_enum,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt) {

    als::layer_id layer = get_combat_layer();

    const combo_move_data* combo_move = my_als->get_combo_move(layer);

    // a move without signals raises its attack at attack_time
    if (!executed_faked_time_attack                 &&
        combo_move                                  &&
        combo_move->flags_1 & combo_flag_non_signal &&
        combo_move->get_attack_time(nullptr) >= 0.0f) {

        f32 duration   = my_als->get_duration(als::base_layer);
        f32 normalized = my_als->get_current_normalized_time(als::base_layer);
        f32 anim_time  = (normalized - (i32)normalized) * duration;

        if (anim_time >= combo_move->get_attack_time(nullptr)) {
            get_actor()->raise_event(als::references::attack.read());

            executed_faked_time_attack = true;
        }
    }

    advance_combat_script(delta_t, combo_move, my_als, cmbt);

    bool attack_raised = (signal_flags & combat_signal_attack) != 0;
    bool opened        = false;

    if (attack_end_pending)
        exit_attack_window(combo_move, my_als, cmbt);

    if (!in_attack_window) {
        f32 since_begin = my_als->get_time_since_signal(als::references::attack_begin.read(), als::base_layer, nullptr, false, nullptr);
        f32 since_end   = my_als->get_time_since_signal(als::references::attack_end.read(),   als::base_layer, nullptr, false, nullptr);

        if (signal_flags & combat_signal_attack_begin ||
            attack_raised                             ||
            (since_begin >= 0.0f && (0.0f > since_end || since_end > since_begin))) {

            enter_attack_window(combo_move, my_als, cmbt);

            opened = true;

            unk_057(combo_move, delta_t);
        }
    } else if (signal_flags & combat_signal_attack_end)
        attack_end_pending = true;

    if (in_attack_window) {
        if (combo_move) {
            update_hit_sphere_positions(my_als);

            vc8::vector<ai_target>         area_targets;
            vc8::vector<arch_base_vhandle> hit_list;

            if (combo_move->unk_14 & combo_unk_14_flag_00100000) {
                chuck::vm::chuck_dynamic_array_t* targets = current_targets->array;

                for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element)
                    hit_list.push_back(*(arch_base_vhandle*)element);
            } else {
                search_area_targets(combo_move, my_als, &area_targets);

                for (ai_target *target = area_targets.begin(), *end = area_targets.end(); target != end; ++target)
                    hit_list.push_back(target->target);
            }

            auto apply_attack = [&]() { // the move's weapons, else its hits
                if (!weapon_to_use[0].valid() && !weapon_to_use[1].valid()) {
                    // the debug damage mode that has the hero kill everything around him once per move
                    if (references::unk_00ffead0.read() && get_actor()->is_hero()) {
                        if (!attack_applied) {
                            retail::sub_5FE3D0(); // damageable search lock

                            if (retail::sub_408FC0((i32)get_actor(), 15.0f, 0x113) > 0) { // damage_interface::find_damageable around the actor, alive, visible, dead and by colgeom
                                vc8::list<damage_interface*>* found = references::found_damageable.read();

                                for (auto *node = found->begin(), *end = found->end(); node != end; node = node->next) {
                                    vector3d blast = unk_035(combo_move, node->value->my_actor->my_handle);

                                    const vector3d &actor_position = get_actor()->my_abs_po->get_position();
                                    const vector3d &owner_position = node->value->my_actor->my_abs_po->get_position();

                                    vector3d delta((f32)((f64)owner_position.x - actor_position.x),
                                                   (f32)((f64)owner_position.y - actor_position.y),
                                                   (f32)((f64)owner_position.z - actor_position.z));

                                    vector3d    direction = math::unitize(delta);
                                    string_hash wounded   = combo_move->get_wounded_category(nullptr);

                                    i32 dealt = node->value->apply_damage_with_combo_web_damage(get_actor(),
                                                                                                nullptr,
                                                                                                100000,
                                                                                                8,
                                                                                                4,
                                                                                                nullptr,
                                                                                                nullptr,
                                                                                                &actor_position,
                                                                                                &direction,
                                                                                                0,
                                                                                                &wounded,
                                                                                                &blast,
                                                                                                num_rb_phys_bones,
                                                                                                nullptr,
                                                                                                nullptr,
                                                                                                false);

                                    if (node->value->my_actor->get_ai_core())
                                        if (auto* their_combat = (cpu_combat_inode*)node->value->my_actor->get_ai_core()->get_info_node(info_node_type_cpu_combat))
                                            their_combat->unk_024(get_actor(), combo_move, nullptr, nullptr, 4, dealt);
                                }
                            }

                            retail::sub_5FE3F0(); // damageable search unlock
                        }

                        return;
                    }
                } else {
                    bool melee = false;

                    if (opened)
                        perform_auto_draw(combo_move);

                    auto* weapons = (ai_weapon_inode*)get_core()->get_info_node(info_node_type_ai_weapon);

                    if (!weapons)
                        return;

                    arch_base_vhandle first_target;

                    chuck::vm::chuck_dynamic_array_t* targets = current_targets->array;

                    for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element)
                        if (((arch_base_vhandle*)element)->resolve()) {
                            first_target = *(arch_base_vhandle*)element;
                            break;
                        }

                    ai_weapon* weapon_pair[2] = { weapons->find_weapon(weapon_to_use[0]),
                                                  weapons->find_weapon(weapon_to_use[1]) };

                    u32 flags_2 = combo_move->flags_2;

                    bool use_if_empty[2] = { (flags_2 & combo_flag_use_weapon_if_empty)      != 0, (flags_2 & combo_flag_use_weapon_2_if_empty)      != 0 };
                    bool use_gun[2]      = { (flags_2 & combo_flag_use_weapon_gun)           != 0, (flags_2 & combo_flag_use_weapon_2_gun)           != 0 };
                    bool use_rocket[2]   = { (flags_2 & combo_flag_use_weapon_rocket)        != 0, (flags_2 & combo_flag_use_weapon_2_rocket)        != 0 };
                    bool use_grenade[2]  = { (flags_2 & combo_flag_use_weapon_grenade)       != 0, (flags_2 & combo_flag_use_weapon_2_grenade)       != 0 };
                    bool use_melee[2]    = { (flags_2 & combo_flag_use_weapon_melee)         != 0, (flags_2 & combo_flag_use_weapon_2_melee)         != 0 };
                    bool holster[2]      = { (flags_2 & combo_flag_holster_weapon_on_attack) != 0, (flags_2 & combo_flag_holster_weapon_2_on_attack) != 0 };

                    bool fired = false;

                    // the script's set_*_fire_info may pick the target and position out of the core's params
                    auto fire = [&](ai_weapon* weapon, i32 index, e_weapon_fire_kind kind, i32 fire_info_func_id, const char* target_name, const char* position_name) {
                        chuck::vm::script_instance* inst = fire_info_func_id != -1 ? script::get_instance(combat_script_inst) : nullptr;

                        if (inst) {
                            f32      answer = 0.0f;
                            f32      slot   = (f32)index;
                            vector3d facing = ((entity_base*)weapon->weapon_entity.resolve())->my_abs_po->get_z_facing();

                            chuck::vm::vm_thread* thread = script::create_thread(fire_info_func_id, inst, get_actor());

                            script::push_value(thread, slot);
                            script::push_value(thread, first_target);
                            script::push_value(thread, facing);
                            script::push_value(thread, current_targets->array);
                            script::push_value(thread, combat_anim_values->array);
                            script::run_thread_with_return(thread, &answer);

                            if (!(answer <= 0.5f)) {
                                string_hash target_key;
                                target_key.initialize(mash::ALLOCATED, target_name);

                                arch_base_vhandle target = get_core()->my_param_block.get_optional_pb_entity(target_key, arch_base_vhandle(), nullptr);

                                string_hash position_key;
                                position_key.initialize(mash::ALLOCATED, position_name);

                                vector3d position = get_core()->my_param_block.get_optional_pb_vector3d(position_key, unset_fire_position, nullptr);

                                ai_weapon::fire(get_actor(), weapon, kind, use_if_empty[index], target, position);

                                return;
                            }
                        }

                        ai_weapon::fire(get_actor(), weapon, kind, use_if_empty[index], first_target, unset_fire_position);
                    };

                    for (i32 index = 0; index < 2; ++index) {
                        if (combo_move->unk_14 & combo_unk_14_flag_80000000 && index != next_weapon_index)
                            continue;

                        ai_weapon* weapon = weapon_pair[index];

                        if (!weapon || !weapon->has_weapon_entity() || !weapon->enabled)
                            continue;

                        if (use_gun[index]) {
                            gun_weapon_interface* gun = weapon->get_gun_weapon_ifc();

                            if (!gun || (!opened && gun->unk_03c > 0.0f))
                                continue;

                            fire(weapon, index, weapon_fire_kind_gun, set_gun_fire_info_func_id, "COMBAT_GUN_FIRE_TARGET", "COMBAT_GUN_FIRE_POS");
                        } else if (use_rocket[index]) {
                            if (!weapon->get_thrown_weapon_ifc() || !opened)
                                continue;

                            fired = true;

                            fire(weapon, index, weapon_fire_kind_rocket, set_rocket_fire_info_func_id, "COMBAT_ROCKET_FIRE_TARGET", "COMBAT_ROCKET_FIRE_POS");
                        } else if (use_grenade[index]) {
                            if (!weapon->get_thrown_weapon_ifc() || !opened)
                                continue;

                            fired = true;

                            fire(weapon, index, weapon_fire_kind_grenade, set_grenade_fire_info_func_id, "COMBAT_GRENADE_FIRE_TARGET", "COMBAT_GRENADE_FIRE_POS");
                        } else {
                            if (use_melee[index]) {
                                melee = true;
                                continue;
                            }

                            if (!opened)
                                continue;

                            fired = true;

                            ai_weapon::fire(get_actor(), weapon, use_if_empty[index], first_target, unset_fire_position);
                        }

                        if (holster[index])
                            weapons->holster_weapon(weapon, nullptr, true);
                    }

                    if (fired && combo_move->unk_14 & combo_unk_14_flag_80000000)
                        next_weapon_index = next_weapon_index == 0 ? 1 : 0;

                    // only a melee weapon goes on to the hits
                    if (!melee) {
                        attack_dodged  = false;
                        attack_blocked = false;
                        attack_missed  = false;

                        cmbt->flags &= ~(cpu_combat_inode_flag_attack_missed | cpu_combat_inode_flag_attack_connected | cpu_combat_inode_flag_attack_blocked);

                        return;
                    }
                }

                bool     landed = false;
                vector3d connect_point;

                for (arch_base_vhandle* handle = hit_list.begin(); handle != hit_list.end();) {
                    auto* target = (entity*)handle->resolve();

                    ++handle;

                    if (target                                                                 &&
                        check_for_target_hit(target, combo_move, my_als, cmbt, &connect_point) &&
                        hit_target(target, combo_move, my_als, cmbt, &connect_point))

                        landed = true;
                }

                if (!landed)
                    spawn_sound(combo_move,
                                get_actor(),
                                combo_move->get_missed_sound_group(nullptr),
                                &get_actor()->my_abs_po->get_position(),
                                spawn_missed_sound_func_id,
                                nullptr);

                if (!jump_connect_applied                                                                  &&
                    combo_move->params.get_optional_pb_float(jump_connect_force_key, 0.0f, nullptr) > 0.0001f &&
                    (attack_connected || combo_move->flags_1 & combo_flag_always_jump)                     &&
                    apply_jump_connect(combo_move))

                    jump_connect_applied = true;
            };

            apply_attack();

            save_prev_hit_sphere_positions(my_als);
        }

        attack_applied = true;

        my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_applied, 1.0f);

        if (combo_move                                                 &&
            combo_move->flags_1 & combo_flag_invulnerable_until_attack &&
            get_actor()->has_ifc(entity_ifc_damage))

            get_actor()->damage_ifc()->flags &= ~damage_ifc_flag_combat_invulnerable;
    }

    // a target dodging or countering this actor stops being one
    if (current_targets->size()) {
        for (auto it = current_targets->begin(); it != current_targets->end();) {
            auto* target  = (entity_base*)(*it).resolve();
            bool  dodging = false;

            if (target && get_actor())
                dodging = is_entity_dodging_or_countering(target, get_actor());

            if ((combo_move && combo_move->unk_18 & combo_unk_18_flag_00040000) || !dodging)
                ++it;
            else {
                current_targets->erase(it);

                if (!current_targets->size())
                    break;
            }
        }
    }

    if (get_actor()->has_ifc(entity_ifc_physical)) {
        physical_interface* physical = get_actor()->physical_ifc();

        physical->get_final_gravity_multiplier(); // the answer goes unused

        if (combo_move && combo_move->flags_1 & combo_flag_use_target_gravity) {
            f32 total = 0.0f;
            f32 count = 0.0f;

            for (auto it = current_targets->begin(), end = current_targets->end(); it != end; ++it) {
                auto* target = (entity_base*)(*it).resolve();

                if (target                               &&
                    target->has_ifc(entity_ifc_physical) &&
                    target->physical_ifc()->is_gravity()) {

                    total = target->physical_ifc()->get_final_gravity_multiplier() + total;
                    count = (f32)((f64)count + 1.0);
                }
            }

            if (count > 0.01f)
                physical->set_combat_gravity_multiplier((f32)((f64)total / count), (f32)((f64)delta_t * 1.5));
        } else {
            bool     found    = false;
            vector3d velocity = physical->get_velocity();

            f32 filter = my_als->get_optional_als_pb_float(gravity_filter_down_only_key, 1.0f, layer, &found);

            if (!(0.0f > velocity.y && found)) {
                filter = my_als->get_optional_als_pb_float(gravity_filter_up_only_key, 1.0f, layer, &found);

                if (!(velocity.y > 0.0f && found))
                    filter = my_als->get_optional_als_pb_float(gravity_filter_key, 1.0f, layer, &found);
            }

            if (found)
                physical->set_combat_gravity_multiplier((f32)((f64)filter * references::unk_00f4d590.read()), (f32)((f64)delta_t * 1.5));
        }
    }

    if (attack_raised)
        exit_attack_window(combo_move, my_als, cmbt);

    // what the hero's reflex dilation leaves alone
    if (combo_move && (combo_move->flags_1 & combo_flag_dodge || combo_move->flags_2 & combo_flag_block)) {
        if (!get_actor()->has_ifc(entity_ifc_time))
            get_actor()->create_time_ifc();

        get_actor()->time_ifc()->ignore_reflex_dilation = (bool)hero_is_a_target;
    } else if (!hero_is_a_target || get_actor()->is_hero()) {
        if (get_actor()->has_ifc(entity_ifc_time))
            get_actor()->time_ifc()->ignore_reflex_dilation = false;
    } else {
        if (!get_actor()->has_ifc(entity_ifc_time))
            get_actor()->create_time_ifc();

        get_actor()->time_ifc()->ignore_reflex_dilation = !attack_applied;
    }
}

// sub_410DB0
void combat_state::fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) {
    f32 duration   = my_als->get_duration(als::base_layer);
    f32 normalized = my_als->get_current_normalized_time(als::base_layer);
    f32 anim_time  = (normalized - (i32)normalized) * duration;

    f32 speed = 1.0f;

    if (als::state_machine_interface* layer = my_als->my_als->get_als_layer(als::base_layer))
        speed = layer->get_speed();

    f32 secondary_speed = 1.0f;

    if (als::state_machine_interface* layer = my_als->my_als->get_als_layer(als::base_layer))
        secondary_speed = layer->get_secondary_speed();

    f32 inv_speed = (f32)(1.0 / (f32)((f64)secondary_speed * speed));

    (*combat_anim_values)[cmbt_anim_time]            = anim_time;
    (*combat_anim_values)[cmbt_anim_duration]        = duration;
    (*combat_anim_values)[cmbt_normalized_anim_time] = normalized;

    // the element after a signal's time to it: that time at the layer's speeds, unless the signal isn't ahead
    auto scale_time_to_signal = [&](u32 raised) {
        (*combat_anim_values)[raised + 2] = (*combat_anim_values)[raised + 1] >= 0.0f ?
            (f32)((f64)(*combat_anim_values)[raised + 1] * inv_speed) :
            (*combat_anim_values)[raised + 1];
    };

    auto fill_signal = [&](u32 raised, u32 signal, string_hash evt) {
        (*combat_anim_values)[raised]     = signal_flags & signal ? 1.0f : 0.0f;
        (*combat_anim_values)[raised + 1] = my_als->get_time_to_signal(evt, als::base_layer, nullptr, false, nullptr);

        scale_time_to_signal(raised);
    };

    fill_signal(cmbt_action_signal_raised,          combat_signal_anim_action,     als::references::anim_action.read());
    fill_signal(cmbt_attack_signal_raised,          combat_signal_attack,          als::references::attack.read());
    fill_signal(cmbt_attack_begin_signal_raised,    combat_signal_attack_begin,    als::references::attack_begin.read());
    fill_signal(cmbt_attack_end_signal_raised,      combat_signal_attack_end,      als::references::attack_end.read());
    fill_signal(cmbt_web_shoot_signal_raised,       combat_signal_web_shoot,       als::references::web_shoot.read());
    fill_signal(cmbt_web_release_signal_raised,     combat_signal_web_release,     als::references::web_release.read());
    fill_signal(cmbt_move_begin_signal_raised,      combat_signal_move_begin,      als::references::move_begin.read());
    fill_signal(cmbt_move_end_signal_raised,        combat_signal_move_end,        als::references::move_end.read());
    fill_signal(cmbt_rotate_begin_signal_raised,    combat_signal_rotate_begin,    als::references::rotate_begin.read());
    fill_signal(cmbt_rotate_end_signal_raised,      combat_signal_rotate_end,      als::references::rotate_end.read());
    fill_signal(cmbt_translate_begin_signal_raised, combat_signal_translate_begin, als::references::translate_begin.read());
    fill_signal(cmbt_translate_end_signal_raised,   combat_signal_translate_end,   als::references::translate_end.read());

    // a move without signals has its attack at attack_time
    if (0.0f > (*combat_anim_values)[cmbt_time_to_attack_signal] &&
        combo_move                                               &&
        combo_move->flags_1 & combo_flag_non_signal              &&
        combo_move->get_attack_time(nullptr) >= 0.0f) {

        f32 time_left = combo_move->get_attack_time(nullptr) - anim_time;

        (*combat_anim_values)[cmbt_time_to_attack_signal] = 0.0f > time_left ? -1.0f : time_left;

        scale_time_to_signal(cmbt_attack_signal_raised);
    }

    (*combat_anim_values)[cmbt_script_move_id] = combo_move ? combo_move->get_script_move_id(nullptr) : -1.0f;
    (*combat_anim_values)[cmbt_damage]         = (f32)get_damage(combo_move, arch_base_vhandle());
    (*combat_anim_values)[cmbt_damage_type]    = (f32)get_damage_type(combo_move, arch_base_vhandle());

    for (u32 button = cmbt_button_jump_triggered; button <= cmbt_button_held; ++button)
        (*combat_anim_values)[button] = 0.0f;

    (*combat_anim_values)[cmbt_unk_47] = -1.0f;

    (*combat_anim_values)[cmbt_strength_test_normalized_amount] = strength_test ?
        ((f32 (__thiscall*)(plr_strength_test*))retail::sub_90C6B0)(strength_test) : // plr_strength_test: the normalized amount
        -1.0f;
}

// sub_409D90
bool combat_state::is_combo_cancelled(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) const {
    bool cancelled = false;

    if (!combo_move)
        return cancelled;

    // a sync anim, strength test or buddy combo whose transition failed, unless the hit that kills it already landed
    if ((combo_move->flags_2 & combo_flag_sync_anim || combo_move->flags_2 & combo_flag_strength_test) &&
        !in_sync_combo_anim                                                                            &&
        !my_als->my_als->get_als_layer(als::base_layer)->did_transition_succeed()                      &&
        (!(combo_move->flags_2 & combo_flag_kill_sync_anim_on_hit) || !attack_applied))

        cancelled = true;

    if (combo_move->flags_2 & combo_flag_buddy_combo                              &&
        !in_buddy_combo_anim                                                      &&
        !my_als->my_als->get_als_layer(als::base_layer)->did_transition_succeed() &&
        (!(combo_move->flags_2 & combo_flag_kill_buddy_combo_on_hit) || !attack_applied))

        return true;

    return cancelled;
}

// sub_40AD50
bool combat_state::unk_031(f32 fall_speed) {
    bool hurt = false;

    if (get_actor()->has_ifc(entity_ifc_damage))
        hurt = retail::sub_6799A0((i32)get_actor()->damage_ifc(), fall_speed, 0.0f, 0, 5) > 0; // damage_interface: fall damage

    auto* my_als = (als_inode*)get_actor()->get_ai_core()->get_info_node(info_node_type_als);

    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_fall_speed, fall_speed);

    return hurt;
}

// slot 32
void combat_state::clear_targets() {
    ((void (__thiscall*)(combat_state*))retail::sub_41A230)(this);
}

// sub_40DDD0
i32 combat_state::get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const {
    i32 type = 3;

    if (!combo_move)
        return type;

    if (combo_move->flags_1 & combo_flag_explosive)
        type = 4;

    if (get_damage_type_func_id == -1)
        return type;

    chuck::vm::script_instance* inst = script::get_instance(combat_script_inst);

    if (!inst)
        return type;

    // the script's type, within 0..7
    f32 script_type = (f32)type;

    chuck::vm::vm_thread* thread = script::create_thread(get_damage_type_func_id, inst, get_actor());

    script::push_value(thread, script_type);
    script::push_value(thread, target);
    script::push_value(thread, current_targets->array);
    script::push_value(thread, combat_anim_values->array);
    script::run_thread_with_return(thread, &script_type);

    type = (i32)script_type;

    if (type < 0)
        return 0;

    if (type > 7)
        return 7;

    return type;
}

// slot 34
i32 combat_state::get_damage(const combo_move_data* combo_move, arch_base_vhandle target) const {
    return ((i32 (__thiscall*)(const combat_state*, const combo_move_data*, arch_base_vhandle))retail::sub_40DEB0)(this, combo_move, target);
}

// slot 35
vector3d combat_state::unk_035(const combo_move_data* combo_move, arch_base_vhandle target) {
    vector3d result;

    ((vector3d* (__thiscall*)(combat_state*, vector3d*, const combo_move_data*, arch_base_vhandle))retail::sub_40E180)(this,
                                                                                                                       &result,
                                                                                                                       combo_move,
                                                                                                                       target);

    return result;
}

// slot 36
void combat_state::enter_custom_camera(const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_416DF0)(this, combo_move);
}

// sub_410CC0
void combat_state::exit_custom_camera() {
    if (!in_custom_cam)
        return;

    references::g_world_ptr.read()->camera_mgr.animated_camera->flags &= ~entity_flag_externally_controlled;

    retail::sub_97BC40((i32)references::game.read(), false, 0, 0.3f, 0, (i32)string_hash().source_hash_code, true); // game::enable_animated_cam

    in_custom_cam = false;
}

// sub_41A060
void combat_state::kill_sync_anim(bool next_move_is_synced) {
    if (next_move_is_synced || !in_sync_combo_anim)
        return;

    chuck::vm::chuck_dynamic_array_t* targets = sync_targets->array;

    for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element)
        release_sync_target((actor*)((arch_base_vhandle*)element)->resolve());

    sync_targets->clear();

    update_initially_standing();

    get_actor()->raise_event(references::ai_combat_sync_end.read());

    in_sync_combo_anim = false;
}

// sub_41F5D0
void combat_state::start_sync_anim(const combo_move_data* combo_move) {
    if (!combo_move)
        return;

    if (!(combo_move->flags_2 & combo_flag_sync_anim) && !(combo_move->flags_2 & combo_flag_strength_test))
        return;

    if (!(combo_move->unk_10 & combo_unk_10_flag_00000010))
        entity_set_facing_and_up(get_actor(), references::unk_00fc1ee0.get(), references::unk_00e6049c.get(), false);

    als::category_id target_category = combo_move->get_target_sync_category(nullptr);
    auto             layer           = (als::layer_id)combo_move->params.get_optional_pb_int(target_sync_layer_key, 0, nullptr);

    if (!target_category.valid())
        return;

    if (current_targets->empty())
        return;

    if (!engage_sync_targets())
        return;

    get_actor()->raise_event(references::ai_combat_sync_begin.read());
    get_actor()->get_als()->force_update(0.0001f, true);

    chuck::vm::chuck_dynamic_array_t* targets = sync_targets->array;

    for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element) {
        auto* target = (actor*)((arch_base_vhandle*)element)->resolve();

        if (!target)
            continue;

        als::animation_logic_system_interface* target_als = target->get_als();

        request_category_transition_helper(target_category, layer, target_als, true, false);
        target_als->force_update(0.0001f, true);

        if (target->has_ifc(entity_ifc_standing))
            target->standing_ifc()->clear_effectively_standing();
    }

    in_sync_combo_anim = true;

    if (get_actor()->has_standing_ifc())
        get_actor()->standing_ifc()->clear_effectively_standing();
}

// sub_40F230
void combat_state::kill_buddy_combo(bool next_move_is_buddy) {
    if (next_move_is_buddy || !in_buddy_combo_anim)
        return;

    get_actor();

    auto* buddy = (actor*)sync_buddy.resolve();
    sync_buddy  = arch_base_vhandle();

    if (buddy) {
        if (buddy->get_als() && buddy->anim_ctrl)
            retail::sub_5F5E90(buddy->anim_ctrl, nullptr, nullptr); // animation_controller::set_master

        if (buddy->has_ifc(entity_ifc_standing) && buddy->standing_ifc()->is_enabled())
            buddy->standing_ifc()->manage_standing(true, 0.5f);

        if (ai_core* buddy_core = buddy->get_ai_core())
            buddy_core->pop_base_machine(references::combat_sync_react_asg.get(), true);

        buddy->raise_event(references::ai_combat_sync_end.read());
    }

    update_initially_standing();

    get_actor()->raise_event(references::ai_combat_sync_end.read());

    in_buddy_combo_anim = false;
}

// sub_4138E0
void combat_state::start_buddy_combo(const combo_move_data* combo_move) {
    if (!combo_move)
        return;

    if (!(combo_move->flags_2 & combo_flag_buddy_combo))
        return;

    als::category_id buddy_category = combo_move->get_buddy_sync_category(nullptr);

    if (!buddy_category.valid())
        return;

    if (current_buddies->empty())
        return;

    arch_base_vhandle buddy_handle = (*current_buddies)[0];
    auto*             buddy        = (actor*)buddy_handle.resolve();

    if (!buddy || !buddy->is_an_actor())
        return;

    als::animation_logic_system_interface* buddy_als = buddy->get_als();

    if (!buddy_als)
        return;

    if (sync_buddy != buddy_handle)
        kill_buddy_combo(false);

    sync_buddy = buddy_handle;

    if (!in_buddy_combo_anim) {
        animation_controller* master_ctrl = get_actor()->anim_ctrl;
        ai_core*              buddy_core  = buddy->get_ai_core();

        if (buddy_core && !buddy_core->attempt_push_base_machine(get_core(), references::combat_sync_react_asg.get(), true))
            return;

        buddy->raise_event(references::ai_combat_sync_begin.read());

        if (buddy_core) {
            auto* car = (ai_car_inode*)buddy_core->get_info_node(info_node_type_ai_car);

            if (car->is_in_car)
                car->force_exit_car(false);
        }

        if (animation_controller* buddy_ctrl = buddy->anim_ctrl)
            retail::sub_5F5E90(buddy_ctrl, (u32**)master_ctrl, nullptr); // animation_controller::set_master
    }

    get_actor()->raise_event(references::ai_combat_sync_begin.read());

    request_category_transition_helper(buddy_category, als::base_layer, buddy_als, true, false);
    buddy_als->force_update(0.0001f, true);

    in_buddy_combo_anim = true;
}

// slot 42
void combat_state::kill_strength_test() {
    ((void (__thiscall*)(combat_state*))retail::sub_40F460)(this);
}

// slot 43
void combat_state::start_strength_test(const combo_move_data*  combo_move,
                                             als_inode*        my_als,
                                             cpu_combat_inode* cmbt,
                                             f32               delta_t) {

    ((void (__thiscall*)(combat_state*,
                         const combo_move_data*,
                         als_inode*,
                         cpu_combat_inode*,
                         f32))retail::sub_421910)(this,
                                                  combo_move,
                                                  my_als,
                                                  cmbt,
                                                  delta_t);
}

// sub_40EDC0
void combat_state::spawn_sound(const combo_move_data* combo_move,
                                     entity*          player,
                                     string_hash      sound_grp,
                               const vector3d*        connect_point,
                                     i32              sound_func_id,
                                     void*            unk) {

    chuck::vm::script_instance* inst = script::get_instance(combat_script_inst);

    // the script's sound function, else the entity posts the group
    if (inst && sound_func_id != -1) {
        chuck::vm::vm_thread* thread = script::create_thread(sound_func_id, inst, get_actor());

        script::push_entity(thread, player);
        script::push_value(thread, *connect_point);
        script::push_value(thread, combat_anim_values->array);
        script::run_thread(thread);

        return;
    }

    if (player->has_ifc(entity_ifc_player))
        player->player_ifc();

    if (player->has_ifc(entity_ifc_sound))
        retail::sub_65FA00((i32)player->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)sound_grp.source_hash_code, false); // sound_interface: post event
}

// sub_40EBA0
void combat_state::spawn_sound_impact(const combo_move_data* combo_move,
                                            entity*          attacker,
                                            entity*          target,
                                      const vector3d*        connect_point,
                                            i32              damage,
                                            bool             blocked) {

    if (attacker->has_ifc(entity_ifc_combo) && attacker->has_ifc(entity_ifc_sound))
        retail::sub_6011E0((i32)attacker->my_ifc_storage->get_ifc(entity_ifc_sound),
                           (i32)combo_count_key.source_hash_code,
                           (f32)(attacker->combo_ifc()->combo_counter + 1)); // sound_interface: set rtpc

    auto* my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);

    string_hash none;
    none.initialize(mash::ALLOCATED);

    string_hash effect = my_als->get_optional_als_pb_hash(hit_effect_key, none, als::base_layer, nullptr);

    if (!target->has_ifc(entity_ifc_sound))
        return;

    string_hash no_effect;
    no_effect.initialize(mash::ALLOCATED, "no_effect");

    if (effect == no_effect)
        return;

    string_hash event;

    if (blocked)
        event = block_key;
    else {
        retail::sub_6011E0((i32)target->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)damage_key.source_hash_code, (f32)damage); // sound_interface: set rtpc

        // the hero tries his own impact first
        if (attacker->is_hero() &&
            retail::sub_65FA00((i32)target->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)impact_hero_key.source_hash_code, false))

            return;

        event = impact_key;
    }

    retail::sub_65FA00((i32)target->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event
}

// slot 46
string_hash combat_state::get_combat_als_category() const {
    string_hash category;

    ((string_hash* (__thiscall*)(const combat_state*, string_hash*))retail::sub_409FA0)(this, &category);

    return category;
}

// sub_40A8F0
als::layer_id combat_state::get_combat_layer() {
    als::layer_id layer = als::base_layer;

    if (auto* basic = (ai_basic_inode*)get_core()->get_info_node(info_node_type_ai_basic))
        layer = basic->combat_layer;

    auto* my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);

    if (!my_als->my_als->has_als_layer(layer))
        return als::base_layer;

    return layer;
}

// sub_5BD2C0
bool combat_state::unk_048(const combo_move_data*, vc8::vector<ai_target>*) {
    return false;
}

// sub_40BF60
bool combat_state::unk_049(      entity*           target,
                           const combo_move_data*  combo_move,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt,
                                 vector3d*         connect_point) {

    vector3d target_point;

    retail::sub_439BA0((i32)&target_point, (u32*)target, (u32*)get_actor()); // the target's point closest to the actor

    vector3d near_point = get_actor()->my_abs_po->get_position();

    if (combo_move->flags_1 & combo_flag_area_of_effect) {
        // within damage_radius and damage_arc of the actor
        vector3d delta((f32)((f64)target_point.x - near_point.x),
                       (f32)((f64)target_point.y - near_point.y),
                       (f32)((f64)target_point.z - near_point.z));

        f32 distance2 = (f32)((f64)delta.y * delta.y + (f64)delta.x * delta.x + (f64)delta.z * delta.z);

        f32 radius       = combo_move->params.get_optional_pb_float(damage_radius_key, 5.0f, nullptr);
        f32 radius_again = combo_move->params.get_optional_pb_float(damage_radius_key, 5.0f, nullptr);

        if (!((f64)radius_again * radius >= distance2))
            return false;

        vector3d facing;
        vector3d up;

        ((vector3d* (__thiscall*)(const combat_state*, vector3d*))retail::sub_4D2450)(this, &facing); // the actor's z row
        ((vector3d* (__thiscall*)(const combat_state*, vector3d*))retail::sub_4D2480)(this, &up);     // the actor's y row

        f32 arc   = combo_move->params.get_optional_pb_float(damage_arc_key, 90.0f, nullptr);
        f32 angle = (f32)retail::sub_402120((f32*)&delta, (f32*)&facing); // angle_between

        if (!((f32)(arc * 0.017453293005625408) >= angle))
            return false;
    } else {
        f32 margin    = get_actor()->is_hero() ? 1.25f : 1.0f;
        f32 range_mod = combo_move->params.get_optional_pb_float(connect_range_mod_key, 0.0f, nullptr);

        if (last_attack_connected)
            range_mod = (f32)((f64)range_mod + 0.5);

        // measured from the actor's colgeom point closest to five meters ahead
        vector3d        position = get_actor()->my_abs_po->get_position();
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        vector3d ahead((f32)((f64)(f32)((f64)forward.x * 5.0) + position.x),
                       (f32)((f64)(f32)((f64)forward.y * 5.0) + position.y),
                       (f32)((f64)(f32)((f64)forward.z * 5.0) + position.z));

        retail::sub_4375D0((i32)&near_point, (u32*)get_actor(), (f32*)&ahead); // the colgeom point closest to a point

        vector3d delta((f32)((f64)target_point.x - near_point.x),
                       (f32)((f64)target_point.y - near_point.y),
                       (f32)((f64)target_point.z - near_point.z));

        f32 distance = (f32)std::sqrt((f64)(f32)((f64)delta.y * delta.y + (f64)delta.x * delta.x + (f64)delta.z * delta.z));
        f64 reach    = (f64)range_mod + margin;

        if (distance > reach) {
            // two more tries; with both actors upright, the height between them doesn't count
            vector3d up = get_actor()->my_abs_po->get_y_facing();

            up.normalize();

            vector3d target_up = target->my_abs_po->get_y_facing();

            target_up.normalize();

            f32  up_dot  = (f32)(up * target_up);
            bool settled = false;

            if (up_dot > 0.8f                                          &&
                get_actor()->has_ifc(entity_ifc_standing)              &&
                get_actor()->standing_ifc()->is_enabled()              &&
                get_actor()->standing_ifc()->is_effectively_standing() &&
                target->has_standing_ifc()                             &&
                target->standing_ifc()->is_enabled()                   &&
                target->standing_ifc()->is_effectively_standing()) {

                f32 along = (f32)(up * delta);

                vector3d flat((f32)((f64)delta.x - (f32)((f64)up.x * along)),
                              (f32)((f64)delta.y - (f32)((f64)up.y * along)),
                              (f32)((f64)delta.z - (f32)((f64)up.z * along)));

                distance = flat.length_float();

                if (!(std::fabs(along) >= 4.0f) && !(distance > reach))
                    settled = true;
            }

            if (!settled) {
                retail::sub_4375D0((i32)&target_point, (u32*)target, (f32*)&near_point); // the colgeom point closest to a point

                delta = vector3d((f32)((f64)target_point.x - near_point.x),
                                 (f32)((f64)target_point.y - near_point.y),
                                 (f32)((f64)target_point.z - near_point.z));

                distance = (f32)std::sqrt((f64)(f32)((f64)delta.y * delta.y + (f64)delta.x * delta.x + (f64)delta.z * delta.z));

                if (distance > reach                                       &&
                    up_dot > 0.8f                                          &&
                    get_actor()->has_standing_ifc()                        &&
                    get_actor()->standing_ifc()->is_effectively_standing() &&
                    target->has_standing_ifc()                             &&
                    target->standing_ifc()->is_effectively_standing()) {

                    f32 along = (f32)(up * delta);

                    vector3d flat((f32)((f64)delta.x - (f32)((f64)up.x * along)),
                                  (f32)((f64)delta.y - (f32)((f64)up.y * along)),
                                  (f32)((f64)delta.z - (f32)((f64)up.z * along)));

                    distance = flat.length_float();
                }
            }
        }

        if (!(reach >= distance))
            return false;

        vector3d facing;
        vector3d up;

        ((vector3d* (__thiscall*)(const combat_state*, vector3d*))retail::sub_4D2450)(this, &facing); // the actor's z row
        ((vector3d* (__thiscall*)(const combat_state*, vector3d*))retail::sub_4D2480)(this, &up);     // the actor's y row

        up.normalize();

        f32 along = (f32)(up * facing);

        vector3d flat_facing((f32)((f64)facing.x - (f32)((f64)up.x * along)),
                             (f32)((f64)facing.y - (f32)((f64)up.y * along)),
                             (f32)((f64)facing.z - (f32)((f64)up.z * along)));

        flat_facing.normalize();

        const vector3d &actor_position = get_actor()->my_abs_po->get_position();

        vector3d to_target((f32)((f64)target_point.x - actor_position.x),
                           (f32)((f64)target_point.y - actor_position.y),
                           (f32)((f64)target_point.z - actor_position.z));

        // flattened by the flat facing's height, not by the target's
        f32 skew = (f32)(up * flat_facing);

        vector3d flat_to_target((f32)((f64)to_target.x - (f32)((f64)up.x * skew)),
                                (f32)((f64)to_target.y - (f32)((f64)up.y * skew)),
                                (f32)((f64)to_target.z - (f32)((f64)up.z * skew)));

        flat_to_target.normalize();

        // right up close any direction hits
        if (!(0.25f > distance)) {
            f32 angle = (f32)retail::sub_402120((f32*)&to_target, (f32*)&facing); // angle_between

            if (!((f32)(combo_move->get_damage_arc(nullptr) * 0.017453293005625408) >= angle)) {
                f32 flat_angle = (f32)retail::sub_402120((f32*)&flat_to_target, (f32*)&flat_facing); // angle_between

                if (!((f32)(combo_move->get_damage_arc(nullptr) * 0.017453293005625408) >= flat_angle))
                    return false;

                if (!(4.0f > std::fabs((f32)(to_target * up))))
                    return false;
            }
        }
    }

    if (unk_051(target, &target_point, &near_point))
        return false;

    vector3d closest;

    retail::sub_4375D0((i32)&closest, (u32*)target, (f32*)&near_point); // the colgeom point closest to a point

    *connect_point = closest;

    return true;
}

// sub_40D290
bool combat_state::unk_050(      entity*           target,
                           const combo_move_data*  combo_move,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt,
                                 vector3d*         connect_point) {

    for (u32 index = 0; index < my_als->my_als->get_als_layer(als::base_layer)->get_num_unk_44(); ++index) {
        als::state_unk_44* sphere = my_als->my_als->get_als_layer(als::base_layer)->get_unk_44(index);

        vector3d sphere_point = get_actor()->my_abs_po->get_position();
        f32      radius       = sphere->radius;
        vector3d target_point;

        if (hit_sphere_prev_valid[index]) {
            // swept from last frame's position
            retail::sub_4366E0((i32)&target_point,
                               (u32*)target,
                               (i32)&hit_sphere_prev_pos[index],
                               (i32)&hit_sphere_pos[index]); // the colgeom point closest to a segment

            retail::sub_7E7980((f32*)&target_point,
                               (f32*)&hit_sphere_prev_pos[index],
                               (f32*)&hit_sphere_pos[index],
                               (f32*)&sphere_point); // the segment's point closest to a point
        } else {
            retail::sub_4375D0((i32)&target_point, (u32*)target, (f32*)&sphere_point); // the colgeom point closest to a point

            sphere_point = hit_sphere_pos[index];
        }

        vector3d offset((f32)((f64)target_point.x - sphere_point.x),
                        (f32)((f64)target_point.y - sphere_point.y),
                        (f32)((f64)target_point.z - sphere_point.z));

        f32 distance2 = (f32)((f64)offset.y * offset.y + (f64)offset.x * offset.x + (f64)offset.z * offset.z);

        if ((f64)radius * radius >= distance2) {
            bool hit = true;

            if (math::math_pi > sphere->max_angle) {
                // the sphere's direction, turned by the actor or by its member
                po frame;

                frame.identity();

                if (sphere->get_member_name().valid()) {
                    string_hash member_name = sphere->get_member_name();

                    auto* self = (conglomerate*)get_actor();

                    self->get_member_abs_po(self->get_member_index(member_name), &frame);
                } else
                    frame = *get_actor()->my_abs_po;

                vector3d direction = frame.non_affine_slow_xform(sphere->direction);

                hit = sphere->max_angle >= (f32)retail::sub_402120((f32*)&offset, (f32*)&direction); // angle_between
            }

            if (hit) {
                *connect_point = target_point;

                if (!unk_051(target, &target_point, &sphere_point))
                    return true;

                continue;
            }
        }

        // a capsule target, against the sphere point
        if (!target->is_an_actor() ||
            !target->get_colgeom()  ||
            target->get_colgeom()->get_type() != collision_geometry_type_capsule)

            continue;

        po*   abs_po   = target->my_abs_po;
        auto* geometry = (collision_capsule*)target->get_colgeom();

        // both ends go through the target's own po, its w column reset in place first
        matrix4x4 &m = abs_po->get_matrix();

        m.x.w = 0.0f;
        m.y.w = 0.0f;
        m.z.w = 0.0f;
        m.w.w = 1.0f;

        vector3d base = abs_po->slow_xform(geometry->rel_cap.base);

        m.x.w = 0.0f;
        m.y.w = 0.0f;
        m.z.w = 0.0f;
        m.w.w = 1.0f;

        vector3d end = abs_po->slow_xform(geometry->rel_cap.end);

        f32      capsule_radius = geometry->rel_cap.radius;
        vector3d closest;

        retail::sub_7E7980((f32*)&sphere_point, (f32*)&base, (f32*)&end, (f32*)&closest); // the segment's point closest to a point

        f32 dx = (f32)((f64)closest.x - sphere_point.x);
        f32 dy = (f32)((f64)closest.y - sphere_point.y);
        f32 dz = (f32)((f64)closest.z - sphere_point.z);

        if (!((f64)capsule_radius * capsule_radius >= (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz)))
            continue;

        *connect_point = closest;

        if (!unk_051(target, &target_point, &sphere_point))
            return true;
    }

    return false;
}

// sub_40DAB0
bool combat_state::unk_051(entity* target, const vector3d* target_point, const vector3d* attacker_point) {
    bool collisions_active = (get_actor()->flags & entity_flag_collisions_active) != 0;

    get_actor()->set_collisions_active(false, false);

    // from the attacker point to a tenth short of the target point
    vector3d to_target((f32)((f64)target_point->x - attacker_point->x),
                       (f32)((f64)target_point->y - attacker_point->y),
                       (f32)((f64)target_point->z - attacker_point->z));

    vector3d dir = math::unitize(to_target);

    vector3d end((f32)((f64)target_point->x - (f32)((f64)dir.x * 0.1000000014901161)),
                 (f32)((f64)target_point->y - (f32)((f64)dir.y * 0.1000000014901161)),
                 (f32)((f64)target_point->z - (f32)((f64)dir.z * 0.1000000014901161)));

    line_info line(*attacker_point, end);
    bool      obstructed = false;

    if (retail::sub_798F50((char*)&line,
                           (i32)references::entfilter_blocks_beams_no_capsules.read(),
                           (i32)references::obbfilter_lineseg_test.read(),
                           nullptr,
                           0)) { // line_info::check_collision

        // anything but the target or what either is attached to
        obstructed = true;

        if (line.hit_entity.resolve() &&
            (line.hit_entity.resolve() == target                      ||
             target->get_conglom_owner() == line.hit_entity.resolve() ||
             target == ((entity_base*)line.hit_entity.resolve())->get_conglom_owner()))

            obstructed = false;
    }

    get_actor()->set_collisions_active(collisions_active, false);

    retail::sub_798F10((u8*)&line); // line_info::~line_info

    return obstructed;
}

// sub_40DD30
bool combat_state::check_for_target_hit(      entity*           target,
                                        const combo_move_data*  combo_move,
                                              als_inode*        my_als,
                                              cpu_combat_inode* cmbt,
                                              vector3d*         connect_point) {

    if (target_was_hit(target->my_handle))
        return false;

    if (combo_move->flags_1 & combo_flag_always_hit)
        return true;

    if (my_als->my_als->get_als_layer(als::base_layer)->get_num_unk_44())
        return unk_050(target, combo_move, my_als, cmbt, connect_point);

    return unk_049(target, combo_move, my_als, cmbt, connect_point);
}

// sub_41DD80
bool combat_state::hit_target(      entity*           target,
                              const combo_move_data*  combo_move,
                                    als_inode*        my_als,
                                    cpu_combat_inode* cmbt,
                                    vector3d*         connect_point) {

    als::layer_id layer       = get_combat_layer();
    ai_core*      target_core = target && target->is_an_actor() ? ((actor*)target)->get_ai_core() : nullptr;

    attack_missed = true;

    i32 wait_answer;

    if (target_core && !ai_core::wait_for_other_ai_retrying(get_core(), target_core, 10, &wait_answer))
        return false;

    bool hit = target                                                                                    &&
               target != get_actor()                                                                     &&
               target->has_ifc(entity_ifc_damage)                                                        &&
               !(target->damage_ifc()->flags & damage_ifc_flag_combat_invulnerable)                      &&
               (get_actor()->is_hero() || !(target->damage_ifc()->flags & damage_ifc_flag_unk_40000000)) &&
               !(target->ext_flags & entity_ext_flag_invulnerable)                                       &&
               (!target->is_hero() || !references::unk_00ffeab6.read());

    // the target is read even when it's null
    i32      damage      = get_damage(combo_move, target->my_handle);
    i32      damage_type = get_damage_type(combo_move, target->my_handle);
    vector3d blast       = unk_035(combo_move, target->my_handle);

    chuck::vm::script_instance* inst = nullptr;

    if (hit) {
        inst = script::get_instance(combat_script_inst);

        if (inst && allow_target_hit_func_id != -1) {
            chuck::vm::vm_thread* thread = script::create_thread(allow_target_hit_func_id, inst, get_actor());

            script::push_entity(thread, target);
            script::push_value(thread, current_targets->array);
            script::push_value(thread, combat_anim_values->array);

            f32 allowed = 1.0f;

            script::run_thread_with_return(thread, &allowed);

            if (!(allowed > 0.0001f))
                hit = false;
        }
    }

    if (hit) {
        static const string_hash force_first_target_hit_as_primary_target_key = string_hash(hash::djb2("force_first_target_hit_as_primary_target"));

        // the attack window's first target
        if (already_hit_targets->empty()) {
            arch_base_vhandle first = target->my_handle;

            if (combo_move->unk_18 & combo_unk_18_flag_00001000) {
                if (current_targets->empty()) {
                    current_targets->push_back(first);

                    unk_063(my_als, (entity*)first.resolve());
                } else if (!(combo_move->flags_2 & combo_flag_sync_anim) && target->my_handle != *current_targets->begin()) {
                    auto end = current_targets->end();

                    for (auto position = ++current_targets->begin(); position != end; ++position) {
                        if (*position == target->my_handle) {
                            current_targets->erase(position);
                            break;
                        }
                    }

                    current_targets->push_front(first);

                    i32 max_targets = combo_move->get_max_targets(nullptr);

                    if (current_targets->size() > max_targets)
                        current_targets->pop_back();

                    unk_063(my_als, (entity*)first.resolve());
                }
            }

            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_first_target_hit_this_frame, 1.0f);
        }

        if (!combo_move || !(combo_move->unk_18 & combo_unk_18_flag_00000400))
            already_hit_targets->push_back(target->my_handle);

        if (combo_move && get_actor()->has_ifc(entity_ifc_player) && combo_move->unk_14 & combo_unk_14_flag_00000040)
            get_actor()->player_ifc()->unk_514(target->my_handle);

        hit = !check_for_dodge(target, chain_size, combo_move, nullptr, nullptr, 4, cmbt);

        if (hit) {
            if (combo_move) {
                if (combo_move->flags_2 & combo_flag_kill_sync_anim_on_hit) {
                    if (target_core)
                        retail::sub_4CC650((i32)get_core(), (u32*)target_core); // release_wait_for_other_ai

                    kill_sync_anim(false);

                    if (target_core)
                        ai_core::wait_for_other_ai_retrying(get_core(), target_core, 10, &wait_answer);
                }

                if (combo_move->flags_2 & combo_flag_kill_buddy_combo_on_hit)
                    kill_buddy_combo(false);
            }

            attack_connected = true;
            attack_missed    = false;

            hit_react_record* react        = my_als->my_als->get_als_layer(layer)->unk_048(false, target);
            hit_react_record* second_react = my_als->my_als->get_als_layer(layer)->unk_048(true, target);

            attack_blocked = check_for_block(target, chain_size, combo_move, react, nullptr, nullptr, 4, cmbt);

            get_actor()->get_ai_core()->flags |= ai_core_flag_attack_connected;

            bool             signal_exists;
            anim_event_param param;

            signal_exists = false;
            param.type    = 0;

            f32 time_to_signal = my_als->get_time_to_signal(references::hit_pause.read(), layer, &signal_exists, false, &param);

            if (signal_exists && 0.0f >= time_to_signal) {
                animation_controller* ctrl     = get_actor()->anim_ctrl;
                f32                   duration = param.type == 2 ? *(f32*)&param.value : 0.15f;

                ((void (__thiscall*)(animation_controller*, f32, f32))retail::sub_5AD320)(get_actor()->anim_ctrl,
                                                                                          ctrl->get_anim_normalized_time(layer),
                                                                                          duration); // animation_controller: hit pause

                get_actor()->get_ai_core()->spawn_hit_pause_effect();
            }

            // a hit dilation signal still ahead, or one the move waits for, is left to the signal callback
            signal_exists = false;
            param.type    = 0;

            time_to_signal = my_als->get_time_to_signal(references::hit_dilation.read(), layer, &signal_exists, false, &param);

            if (signal_exists) {
                if (!(0.0f >= time_to_signal) || hit_dilation_waits_for_signal)
                    hit_dilation_pending = true;
                else {
                    if (param.type == 4)
                        apply_hit_dilation_preset((i32)param.value, 1);
                    else {
                        combat_dilation_info info;

                        info.duration = 0.125f;
                        info.dilation = 0.15f;

                        if (!get_actor()->has_ifc(entity_ifc_time))
                            get_actor()->create_time_ifc();

                        get_actor()->time_ifc()->set_combat_dilation(info);
                    }

                    get_actor()->get_ai_core()->spawn_hit_pause_effect();
                }
            }

            signal_exists = false;
            param.type    = 0;

            time_to_signal = my_als->get_time_to_signal(references::hit_dilation_target.read(), layer, &signal_exists, false, &param);

            if (signal_exists) {
                if (!(0.0f >= time_to_signal) || hit_dilation_waits_for_signal)
                    hit_dilation_pending = true;
                else {
                    if (param.type == 4)
                        apply_hit_dilation_preset((i32)param.value, 2);
                    else {
                        combat_dilation_info info;

                        info.duration = 0.125f;
                        info.dilation = 0.15f;

                        if (current_targets && current_targets->size() > 0)
                            for (auto it = current_targets->begin(); it != current_targets->end(); ++it) {
                                auto* dilated = (entity_base*)(*it).resolve();

                                if (!dilated)
                                    continue;

                                if (!dilated->has_ifc(entity_ifc_time))
                                    dilated->create_time_ifc();

                                dilated->time_ifc()->set_combat_dilation(info);
                            }
                    }

                    get_actor()->get_ai_core()->spawn_hit_pause_effect();
                }
            }

            signal_exists = false;
            param.type    = 0;

            time_to_signal = my_als->get_time_to_signal(references::hit_dilation_world.read(), layer, &signal_exists, false, &param);

            if (signal_exists) {
                if (!(0.0f >= time_to_signal) || hit_dilation_waits_for_signal)
                    hit_dilation_pending = true;
                else {
                    if (param.type == 4)
                        apply_hit_dilation_preset((i32)param.value, 0);
                    else {
                        combat_dilation_info info;

                        info.duration = 0.125f;
                        info.dilation = 0.15f;

                        ((time_interface*)retail::sub_968660((u32*)&references::g_world_ptr.read()->time_mgr))->set_combat_dilation(info); // wds_time_manager: the world's time interface
                    }

                    get_actor()->get_ai_core()->spawn_hit_pause_effect();
                }
            }

            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_hit_target,       1.0f);
            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_connected, 1.0f);
            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_missed,    0.0f);
            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_dodged,    0.0f);
            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_blocked,   attack_blocked ? 1.0f : 0.0f);

            i32 dealt = 0;

            const vector3d &actor_position  = get_actor()->my_abs_po->get_position();
            const vector3d &target_position = target->my_abs_po->get_position();

            vector3d delta((f32)((f64)target_position.x - actor_position.x),
                           (f32)((f64)target_position.y - actor_position.y),
                           (f32)((f64)target_position.z - actor_position.z));

            vector3d direction = math::unitize(delta);

            if (!attack_blocked) {
                f32 damage_value = (f32)damage;

                (*combat_anim_values)[cmbt_damage]      = damage_value;
                (*combat_anim_values)[cmbt_damage_type] = (f32)damage_type;

                if (!inst || hit_target_func_id == -1) {
                    bool friendly = false;

                    if (combo_move->unk_10 & combo_unk_10_flag_00200000 && target_core) {
                        auto their_team = (ai_team::e_team)retail::sub_42B120((i32)target_core);
                        auto my_team    = (ai_team::e_team)retail::sub_42B120((i32)get_core());

                        if (my_team != ai_team::num_teams && their_team != ai_team::num_teams && ai_team::is_friend(my_team, their_team))
                            friendly = true;
                    }

                    if (target->has_damage_ifc() && !friendly) {
                        damage_interface* their_damage = target->damage_ifc();
                        string_hash       wounded      = combo_move->get_wounded_category(nullptr);

                        dealt = their_damage->apply_damage_with_combo_web_damage(get_actor(),
                                                                                 nullptr,
                                                                                 damage,
                                                                                 8,
                                                                                 damage_type,
                                                                                 react,
                                                                                 second_react,
                                                                                 &get_actor()->my_abs_po->get_position(),
                                                                                 &direction,
                                                                                 cancel_target_death_immunity ? 8 : 0,
                                                                                 &wounded,
                                                                                 &blast,
                                                                                 num_rb_phys_bones,
                                                                                 combo_move,
                                                                                 nullptr,
                                                                                 false);

                        if (combo_move->flags_1 & combo_flag_combo_breaker && their_damage->flags & damage_ifc_flag_combat_engaged)
                            their_damage->flags |= damage_ifc_flag_combat_combo_breaker;
                    }
                } else {
                    // the script deals the damage
                    chuck::vm::vm_thread* thread = script::create_thread(hit_target_func_id, inst, get_actor());

                    script::push_entity(thread, target);
                    script::push_value(thread, current_targets->array);
                    script::push_value(thread, combat_anim_values->array);

                    f32 script_damage = damage_value;

                    script::run_thread_with_return(thread, &script_damage);

                    dealt = (i32)((f64)script_damage + 0.5);

                    if (combo_move->flags_1 & combo_flag_combo_breaker &&
                        target->has_damage_ifc()                       &&
                        target->damage_ifc()->flags & damage_ifc_flag_combat_engaged)

                        target->damage_ifc()->flags |= damage_ifc_flag_combat_combo_breaker;
                }

                ai_team::add_attack_meter(get_core(), 1.0f);

                if (target_core)
                    ai_team::add_hurt_meter(target_core, 1.0f);
            }

            (*combat_anim_values)[cmbt_damage] = (f32)dealt;

            if (attack_blocked) {
                if (combo_move)
                    spawn_sound_impact(combo_move, get_actor(), target, connect_point, dealt, true);

                if (inst && spawn_blocked_fx_func_id != -1) {
                    chuck::vm::vm_thread* thread = script::create_thread(spawn_blocked_fx_func_id, inst, get_actor());

                    script::push_entity(thread, target);
                    script::push_value(thread, *connect_point);
                    script::push_value(thread, combat_anim_values->array);
                    script::run_thread(thread);
                }
            } else {
                if (combo_move)
                    spawn_sound_impact(combo_move, get_actor(), target, connect_point, dealt, false);

                ((bool (__cdecl*)(entity*, entity*, als_inode*, const vector3d*))retail::sub_4A2D00)(get_actor(), target, my_als, connect_point); // the hit effects
            }

            if (!attack_blocked && target->is_an_actor() && ((actor*)target)->get_ai_core()) {
                auto* their_combat = (cpu_combat_inode*)((actor*)target)->get_ai_core()->get_info_node(info_node_type_cpu_combat);

                if (their_combat)
                    their_combat->unk_024(get_actor(), combo_move, nullptr, nullptr, 4, dealt);
            }

            if (cmbt) {
                cmbt->flags = (cmbt->flags & ~cpu_combat_inode_flag_attack_missed) | cpu_combat_inode_flag_attack_connected;

                cmbt->set_attack_blocked(attack_blocked);
            }

            // the hero gets what he hits to throw, held at the connect point or at a conglomerate's sync member nearest to it
            if (target->has_ifc(entity_ifc_manip_obj) && get_actor()->is_hero() && get_actor()->has_player_ifc()) {
                get_actor()->player_ifc()->unk_064(target->my_handle);
                get_actor()->player_ifc()->unk_066(*connect_point);

                if (target->is_a_conglomerate()) {
                    i32 nearest_index    = -1;
                    f32 nearest_distance = 0.0f;

                    for (i32 index = 0; index < 10; ++index) {
                        const string_hash &bone = als::sync_bone_ids[index];

                        if (!((conglomerate*)target)->has_member_for_linking(bone))
                            continue;

                        auto* member = ((conglomerate*)target)->get_member_for_linking(bone);

                        const vector3d &member_position = member->my_abs_po->get_position();

                        vector3d offset((f32)((f64)connect_point->x - member_position.x),
                                        (f32)((f64)connect_point->y - member_position.y),
                                        (f32)((f64)connect_point->z - member_position.z));

                        f32 distance = offset.length_float();

                        if (nearest_distance > distance || nearest_index < 0) {
                            nearest_distance = distance;
                            nearest_index    = index;

                            get_actor()->player_ifc()->unk_066(member->my_abs_po->get_position());
                        }
                    }
                }
            }
        } else {
            my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_combat_attack_dodged, 1.0f);

            if (combo_move)
                spawn_sound(combo_move,
                            target,
                            combo_move->get_dodged_sound_group(nullptr),
                            &target->my_abs_po->get_position(),
                            spawn_dodged_sound_func_id,
                            nullptr);
        }

        // a landed hit, or a dodge that counts as one, stops both sides
        if (hit || combo_move->flags_2 & combo_flag_dodge_counts_as_hit) {
            if (target->has_ifc(entity_ifc_physical)) {
                if (combo_move->flags_1 & combo_flag_target_connect_cancel_all_velocity)
                    target->physical_ifc()->cancel_all_velocity(references::unk_00fc1ee0.get(), true);
                else if (combo_move->get_target_cancel_velocity_dir(nullptr).length2_float() > 0.0001f) {
                    bool both_dir = (combo_move->flags_1 & combo_flag_target_cancel_velocity_dir_both_ways) != 0;

                    target->physical_ifc()->cancel_all_velocity(combo_move->get_target_cancel_velocity_dir(nullptr), both_dir);
                }
            }

            if (get_actor()->has_ifc(entity_ifc_physical)) {
                if (combo_move->flags_1 & combo_flag_self_connect_cancel_all_velocity)
                    get_actor()->physical_ifc()->cancel_all_velocity(references::unk_00fc1ee0.get(), true);
                else if (combo_move->get_self_cancel_velocity_dir(nullptr).length2_float() > 0.0001f) {
                    bool both_dir = (combo_move->flags_1 & combo_flag_self_cancel_velocity_dir_both_ways) != 0;

                    get_actor()->physical_ifc()->cancel_all_velocity(combo_move->get_self_cancel_velocity_dir(nullptr), both_dir);
                }

                // neither side standing: both take the larger velocity, per axis
                if (combo_move->flags_1 & combo_flag_equalize_velocity                                                     &&
                    !(get_actor()->has_ifc(entity_ifc_standing) && get_actor()->standing_ifc()->is_effectively_standing()) &&
                    !(target->has_standing_ifc() && target->standing_ifc()->is_effectively_standing())) {

                    vector3d increment = get_actor()->physical_ifc()->velocity_increment;
                    vector3d velocity  = get_actor()->physical_ifc()->get_velocity();

                    vector3d mine(increment.x + velocity.x,
                                  increment.y + velocity.y,
                                  increment.z + velocity.z);

                    vector3d theirs;

                    // the actor's velocity again, not the target's
                    if (target->has_physical_ifc()) {
                        increment = get_actor()->physical_ifc()->velocity_increment;
                        velocity  = get_actor()->physical_ifc()->get_velocity();

                        theirs = vector3d(increment.x + velocity.x,
                                          increment.y + velocity.y,
                                          increment.z + velocity.z);
                    } else
                        theirs = references::unk_00fc1ee0.get();

                    if (theirs.length2_float() >= 0.01f) {
                        f32 x = std::fabs(mine.x) > std::fabs(theirs.x) ? mine.x : theirs.x;
                        f32 y = mine.y > theirs.y ? mine.y : theirs.y;
                        f32 z = std::fabs(mine.z) <= std::fabs(theirs.z) ? theirs.z : mine.z;

                        get_actor()->physical_ifc()->add_velocity_increment(vector3d(x - mine.x, y - mine.y, z - mine.z), false);

                        if (target->has_physical_ifc())
                            target->physical_ifc()->add_velocity_increment(vector3d(x - theirs.x, y - theirs.y, z - theirs.z), false);
                    } else
                        get_actor()->physical_ifc()->cancel_all_velocity(references::unk_00fc1ee0.get(), true);
                }
            }
        }
    }

    if (target_core)
        retail::sub_4CC650((i32)get_core(), (u32*)target_core); // release_wait_for_other_ai

    return hit;
}

// slot 54
bool combat_state::target_was_hit(arch_base_vhandle target) {
    return ((bool (__thiscall*)(combat_state*, arch_base_vhandle))retail::sub_4129A0)(this, target);
}

// slot 55
bool combat_state::apply_jump_connect(const combo_move_data* combo_move) {
    return ((bool (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_40AE00)(this, combo_move);
}

// slot 56
bool combat_state::unk_056(const combo_move_data* combo_move) {
    return ((bool (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_40B160)(this, combo_move);
}

// slot 57
void combat_state::unk_057(const combo_move_data* combo_move, f32 unk) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*, f32))retail::sub_40B990)(this, combo_move, unk);
}

// slot 58
void combat_state::unk_058() {
    ((void (__thiscall*)(combat_state*))retail::sub_4130C0)(this);
}

// slot 59
void combat_state::unk_059(arch_base_vhandle target) {
    ((void (__thiscall*)(combat_state*, arch_base_vhandle))retail::sub_40B6F0)(this, target);
}

// slot 60
void combat_state::unk_060(f32 unk, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, f32, const combo_move_data*))retail::sub_413380)(this, unk, combo_move);
}

// slot 61
void combat_state::enter_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*, als_inode*, cpu_combat_inode*))retail::sub_418960)(this, combo_move, my_als, cmbt);
}

// sub_418B40
void combat_state::exit_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt) {
    attack_end_pending = false;

    for (u32 sphere = 0; sphere < my_als->my_als->get_als_layer(als::base_layer)->get_num_unk_44(); ++sphere)
        hit_sphere_prev_valid[sphere] = false;

    if (combo_move) {
        if (get_actor()->has_ifc(entity_ifc_standing)) {
            if (combo_move->unk_14 & combo_unk_14_flag_00000001)
                get_actor()->standing_ifc()->flags &= ~standing_interface_flag_combat_disabled;

            if (combo_move->unk_14 & combo_unk_14_flag_00000002)
                get_actor()->standing_ifc()->flags &= ~standing_interface_flag_unk_00001000;
        }

        if (get_actor()->has_ifc(entity_ifc_physical) && combo_move->unk_14 & combo_unk_14_flag_00000004)
            get_actor()->physical_ifc()->flags &= ~physical_interface_flag_unk_00000200;
    }

    already_hit_targets->clear();

    in_attack_window = false;
}

// slot 63
void combat_state::unk_063(als_inode* my_als, entity* target) {
    ((void (__thiscall*)(combat_state*, als_inode*, entity*))retail::sub_40F930)(this, my_als, target);
}

// sub_40E9E0
bool combat_state::check_for_dodge(      entity*                    target,
                                         i32                        chain_size,
                                   const combo_move_data*           combo_move,
                                         weapon_interface_extender* weapon,
                                         entity*                    weapon_entity,
                                         i32                        usage_type,
                                         cpu_combat_inode*          cmbt) {

    if (!(combo_move->flags_2 & combo_flag_allow_target_dodge) && !get_actor()->has_ifc(entity_ifc_player))
        return false;

    if (!target || !target->has_ifc(entity_ifc_damage) || !target->is_alive())
        return false;

    if (!target->damage_ifc()->check_for_dodge(get_actor(), chain_size, combo_move, weapon, weapon_entity, usage_type))
        return false;

    if (cmbt)
        cmbt->flags |= cpu_combat_inode_flag_attack_dodged;

    if (target->is_an_actor() && ((actor*)target)->get_ai_core())
        ai_team::add_attack_meter(((actor*)target)->get_ai_core(), 1.0f);

    attack_dodged = true;

    return true;
}

// sub_40EAD0
bool combat_state::check_for_block(      entity*                    target,
                                         i32                        chain_size,
                                   const combo_move_data*           combo_move,
                                         hit_react_record*          react,
                                         weapon_interface_extender* weapon,
                                         entity*                    weapon_entity,
                                         i32                        usage_type,
                                         cpu_combat_inode*          cmbt) {

    if (!target || !target->has_ifc(entity_ifc_damage) || !target->is_alive())
        return false;

    if (!target->damage_ifc()->check_for_block(get_actor(), chain_size, combo_move, react, weapon, weapon_entity, usage_type))
        return false;

    if (cmbt)
        cmbt->flags |= cpu_combat_inode_flag_attack_blocked;

    if (target->is_an_actor() && ((actor*)target)->get_ai_core())
        ai_team::add_attack_meter(((actor*)target)->get_ai_core(), 1.0f);

    attack_blocked = true;

    return true;
}

// sub_411870
void combat_state::start_combat_script(f32 delta_t) {
    auto*                  my_als     = (als_inode*)get_core()->get_info_node(info_node_type_als);
    const combo_move_data* combo_move = my_als->get_combo_move(get_combat_layer());

    // whether the running instance came from the executable this move names
    bool same_exec = false;

    chuck::vm::script_instance* running = script::get_instance(combat_script_inst);

    if (running && combo_move && *combo_move->get_script_exe(nullptr)) {
        string_hash running_exec;
        running_exec.initialize(mash::ALLOCATED, running->parent->parent->name.c_str());

        string_hash wanted_exec;
        wanted_exec.initialize(mash::ALLOCATED, combo_move->get_script_exe(nullptr));

        same_exec = wanted_exec == running_exec;
    }

    // a move that keeps the script object hands the running instance its move_transition instead
    if (combo_move && combo_move->flags_2 & combo_flag_maintain_script_obj) {
        if (same_exec) {
            if (chuck::vm::script_instance* kept = script::get_instance(combat_script_inst)) {
                string_hash kept_object = kept->parent->name;

                string_hash wanted_object;
                wanted_object.initialize(mash::ALLOCATED, combo_move->get_script_obj(nullptr));

                if (kept_object == wanted_object) {
                    if (move_transition_func_id != -1) {
                        fill_combat_anim_values(combo_move, my_als, delta_t);

                        chuck::vm::vm_thread* thread = script::create_thread(move_transition_func_id, kept, get_actor());

                        script::push_value(thread, combo_move->get_script_move_id(nullptr));
                        script::push_value(thread, current_targets->array);
                        script::push_value(thread, combat_anim_values->array);
                        script::run_thread(thread);
                    }

                    return;
                }
            }
        } else if (running)
            script::unload_script_exec(running->parent->parent->name.c_str(), get_actor());
    }

    kill_combat_script(!same_exec);

    if (!combo_move || !*combo_move->get_script_exe(nullptr))
        return;

    if (!same_exec)
        script::load_script_exec(combo_move->get_script_exe(nullptr), &combat_script_exec_prefix, get_actor());

    if (!combat_script_exec_prefix.is_set())
        return;

    chuck::vm::script_object* object;

    {
        mash::string exec_name(combo_move->get_script_exe(nullptr));

        string_hash object_name;
        object_name.initialize(mash::ALLOCATED, combo_move->get_script_obj(nullptr));

        object = script::find_object(object_name, exec_name, &combat_script_exec_prefix);
    }

    actor* owner = get_actor();

    if (!object) {
        script::unload_script_exec(combo_move->get_script_exe(nullptr), owner);
        return;
    }

    mash::string inst_name(mash::string::fmt, "0x%08x_%s", owner->id.source_hash_code, combo_move->get_script_obj(nullptr));

    string_hash inst_hash;
    inst_hash.initialize(mash::ALLOCATED, inst_name.c_str());

    chuck::vm::vm_thread*       constructor_thread = nullptr;
    chuck::vm::script_instance* inst               = script::find_instance(inst_hash, object);

    if (inst)
        script::reconstruct_instance(inst, &constructor_thread);
    else
        inst = script::add_instance(mash::string(inst_name.c_str()), object, &constructor_thread, get_actor(), chuck::vm::script_instance_stack_size_large);

    combat_script_inst = script::get_instance_handle(inst);

    script::push_value(constructor_thread, get_actor()->my_handle);
    script::run_thread(constructor_thread);

    // retail's lookup order, which isn't the members'
    const struct {
        const char*        fullname;
        i32 combat_state::* func_id;
    } functions[] { { "init_move(entity[],num[])",                                  &combat_state::init_move_func_id                 },
                    { "frame_advance(num,entity[],num[])",                          &combat_state::frame_advance_func_id             },
                    { "hit_target(entity,entity[],num[])",                          &combat_state::hit_target_func_id                },
                    { "allow_target_hit(entity,entity[],num[])",                    &combat_state::allow_target_hit_func_id          },
                    { "spawn_hit_fx(entity,vector3d,num[])",                        &combat_state::spawn_hit_fx_func_id              },
                    { "spawn_blocked_fx(entity,vector3d,num[])",                    &combat_state::spawn_blocked_fx_func_id          },
                    { "spawn_hit_sound(entity,vector3d,num[])",                     &combat_state::spawn_hit_sound_func_id           },
                    { "spawn_attack_sound(entity,vector3d,num[])",                  &combat_state::spawn_attack_sound_func_id        },
                    { "spawn_missed_sound(entity,vector3d,num[])",                  &combat_state::spawn_missed_sound_func_id        },
                    { "spawn_dodged_sound(entity,vector3d,num[])",                  &combat_state::spawn_dodged_sound_func_id        },
                    { "spawn_blocked_sound(entity,vector3d,num[])",                 &combat_state::spawn_blocked_sound_func_id       },
                    { "spawn_attack_window_sound(entity,vector3d,num[])",           &combat_state::spawn_attack_window_sound_func_id },
                    { "get_als_facing(vector3d,num,entity[],num[])",                &combat_state::get_als_facing_func_id            },
                    { "get_als_up(vector3d,num,entity[],num[])",                    &combat_state::get_als_up_func_id                },
                    { "get_als_position(vector3d,num,entity[],num[])",              &combat_state::get_als_position_func_id          },
                    { "get_jump_launch_vector(vector3d,num,entity[],num[])",        &combat_state::get_jump_launch_vector_func_id    },
                    { "get_blast_vector(vector3d,entity,num,entity[],num[])",       &combat_state::get_blast_vector_func_id          },
                    { "kill_move()",                                                &combat_state::kill_move_func_id                 },
                    { "custom_camera(entity,num,entity[],num[])",                   &combat_state::custom_camera_func_id             },
                    { "custom_camera_engaged(num,entity[],num[])",                  &combat_state::custom_camera_engaged_func_id     },
                    { "combat_move(num,entity[],num[])",                            &combat_state::combat_move_func_id               },
                    { "move_transition(num,entity[],num[])",                        &combat_state::move_transition_func_id           },
                    { "strength_test_values(num,entity[],num[],num[])",             &combat_state::strength_test_values_func_id      },
                    { "playback_speed(num,num,entity[],num[])",                     &combat_state::playback_speed_func_id            },
                    { "get_damage(num,entity,entity[],num[])",                      &combat_state::get_damage_func_id                },
                    { "get_damage_type(num,entity,entity[],num[])",                 &combat_state::get_damage_type_func_id           },
                    { "set_gun_fire_info(num,entity,vector3d,entity[],num[])",      &combat_state::set_gun_fire_info_func_id         },
                    { "set_rocket_fire_info(num,entity,vector3d,entity[],num[])",   &combat_state::set_rocket_fire_info_func_id      },
                    { "set_grenade_fire_info(num,entity,vector3d,entity[],num[])",  &combat_state::set_grenade_fire_info_func_id     },
                    { "update_target_lists(entity[],entity[],num[])",               &combat_state::update_target_lists_func_id       },
                    { "get_auto_calc_target_pos(vector3d,num,entity[],num[])",      &combat_state::get_auto_calc_target_pos_func_id  },
                    { "get_auto_calc_target_dir(vector3d,num,entity[],num[])",      &combat_state::get_auto_calc_target_dir_func_id  } };

    for (const auto &function : functions) {
        string_hash fullname;
        fullname.initialize(mash::ALLOCATED, function.fullname);

        this->*function.func_id = script::find_function(fullname, object);
    }
}

// sub_410B90
void combat_state::kill_combat_script(bool unload_exec) {
    if (chuck::vm::script_instance* inst = script::get_instance(combat_script_inst)) {
        if (kill_move_func_id != -1)
            script::run_thread(script::create_thread(kill_move_func_id, inst, get_actor()));

        chuck::vm::script_object* object = inst->parent;

        script::destroy_instance(inst, true);

        if (unload_exec) {
            script::unload_script_exec(object, get_actor());

            combat_script_exec_prefix.set(string_hash(), 0);
        }
    }

    combat_script_inst = arch_base_vhandle();

    // start_combat_script's lookup order
    init_move_func_id                 = -1;
    frame_advance_func_id             = -1;
    hit_target_func_id                = -1;
    allow_target_hit_func_id          = -1;
    spawn_hit_fx_func_id              = -1;
    spawn_blocked_fx_func_id          = -1;
    spawn_hit_sound_func_id           = -1;
    spawn_attack_sound_func_id        = -1;
    spawn_missed_sound_func_id        = -1;
    spawn_dodged_sound_func_id        = -1;
    spawn_blocked_sound_func_id       = -1;
    spawn_attack_window_sound_func_id = -1;
    get_als_facing_func_id            = -1;
    get_als_up_func_id                = -1;
    get_als_position_func_id          = -1;
    get_jump_launch_vector_func_id    = -1;
    get_blast_vector_func_id          = -1;
    kill_move_func_id                 = -1;
    custom_camera_func_id             = -1;
    custom_camera_engaged_func_id     = -1;
    combat_move_func_id               = -1;
    move_transition_func_id           = -1;
    strength_test_values_func_id      = -1;
    playback_speed_func_id            = -1;
    get_damage_func_id                = -1;
    get_damage_type_func_id           = -1;
    set_gun_fire_info_func_id         = -1;
    set_rocket_fire_info_func_id      = -1;
    set_grenade_fire_info_func_id     = -1;
    update_target_lists_func_id       = -1;
    get_auto_calc_target_pos_func_id  = -1;
    get_auto_calc_target_dir_func_id  = -1;
}

// sub_417520
void combat_state::add_combat_thread_info(combat_thread_info* info) {
    engine_lock_scope lock(&references::combat_thread_info_lock.get());

    vc8::list<combat_thread_info*>* infos = references::combat_thread_infos.read();

    if (!infos) {
        infos = new (mm::allocate(sizeof(vc8::list<combat_thread_info*>))) vc8::list<combat_thread_info*>();

        references::combat_thread_infos.write(infos);
    }

    infos->push_front(info);
}

// sub_4175F0
void combat_state::remove_combat_thread_info(combat_thread_info* info) {
    engine_lock_scope lock(&references::combat_thread_info_lock.get());

    vc8::list<combat_thread_info*>* infos = references::combat_thread_infos.read();

    if (!infos)
        return;

    for (auto* node = infos->begin(); node != infos->end(); node = node->next)
        if (node->value == info) {
            delete node->value;

            references::combat_thread_infos.read()->erase(node);

            break;
        }

    infos = references::combat_thread_infos.read();

    if (infos->empty()) {
        infos->~list();
        mm::free(infos);

        references::combat_thread_infos.write(nullptr);
    }
}

// sub_41F7B0
void combat_state::push_target(arch_base_vhandle targ) {
    engine_lock_scope lock(&references::target_list_lock.get());

    auto* target = (entity_base*)targ.resolve();

    if (target && target != get_actor()) {
        current_targets->push_back(targ);

        if (target->is_hero())
            hero_is_a_target = true;
    }
}

// sub_416D60
bool combat_state::is_a_target(arch_base_vhandle targ) const {
    engine_lock_scope lock(&references::target_list_lock.get());

    if (current_targets->size())
        for (auto it = current_targets->begin(); it != current_targets->end(); ++it)
            if (*it == targ)
                return true;

    return false;
}

// sub_410A90
bool combat_state::is_entity_dodging_or_countering(entity_base* target,
                                                   entity_base* attacker,
                                                   bool*        dodging,
                                                   bool*        countering) {

    arch_base_vhandle against;

    if (!target || !attacker || target == attacker || !target->is_an_actor())
        return false;

    ai_core* core = ((actor*)target)->get_ai_core();

    if (!core)
        return false;

    bool in_dodge;
    bool in_counter;

    if (target->has_player_ifc()) {
        in_dodge   = target->player_ifc()->in_dodge_move;
        in_counter = target->player_ifc()->in_counter_attack_move;
        against    = target->player_ifc()->combat_move_target;
    } else {
        auto* cmbt = (cpu_combat_inode*)core->get_info_node(info_node_type_cpu_combat);

        if (!cmbt)
            return false;

        in_dodge   = cmbt->flags & cpu_combat_inode_flag_in_dodge_move;
        in_counter = cmbt->flags & cpu_combat_inode_flag_in_counter_attack_move;
        against    = cmbt->combat_move_target;
    }

    if (!in_dodge && !in_counter)
        return false;

    if (dodging)
        *dodging = in_dodge;

    if (countering)
        *countering = in_counter;

    return against.null() || against.resolve() == attacker;
}

i32 combat_state::internal_auto_calc_targets(const combo_move_data* combo_move, vc8::vector<ai_target>* targets) {
    return ((i32 (__thiscall*)(combat_state*, const combo_move_data*, vc8::vector<ai_target>*))retail::sub_41BF10)(this, combo_move, targets);
}

// sub_40A000
void combat_state::set_delay_stun_recovery(const combo_move_data* combo_move) {
    if (combo_move && combo_move->flags_1 & combo_flag_delay_stun_recovery) {
        if (!delaying_stun) {
            delaying_stun = true;

            damage_interface::push_delay_stun_recovery();
        }
    } else if (delaying_stun) {
        delaying_stun = false;

        damage_interface::pop_delay_stun_recovery();
    }
}

// sub_40EEC0
void combat_state::perform_auto_draw(const combo_move_data* combo_move) {
    if (weapon_to_use[0].valid() || weapon_to_use[1].valid())
        if (auto* weapons = (ai_weapon_inode*)get_core()->get_info_node(info_node_type_ai_weapon)) {
            ai_weapon* weapon       = weapons->find_weapon(weapon_to_use[0]);
            ai_weapon* other_weapon = weapons->find_weapon(weapon_to_use[1]);

            if (auto_draw_move)
                weapons->draw_weapon(weapon, other_weapon, true);
        }

    auto_draw_move = false;
}

// sub_40EF70
void combat_state::perform_auto_holster(const combo_move_data* combo_move) {
    if (weapon_to_use[0].valid() || weapon_to_use[1].valid())
        if (auto* weapons = (ai_weapon_inode*)get_core()->get_info_node(info_node_type_ai_weapon)) {
            ai_weapon* weapon       = weapons->find_weapon(weapon_to_use[0]);
            ai_weapon* other_weapon = weapons->find_weapon(weapon_to_use[1]);

            if (auto_holster_move)
                weapons->holster_weapon(weapon, other_weapon, true);
        }

    auto_holster_move = false;
}

// sub_412E30
void combat_state::update_camera(      f32               delta_t,
                                 const combo_move_data*  combo_move,
                                       als_inode*        my_als,
                                       cpu_combat_inode* cmbt) {

    // without a camera signal this frame the script says whether its camera is engaged
    if (!(signal_flags & (combat_signal_animated_camera_on | combat_signal_animated_camera_off)) &&
        custom_camera_func_id         != -1                                                     &&
        custom_camera_engaged_func_id != -1) {

        bool disengaged = false;

        if (chuck::vm::script_instance* inst = script::get_instance(combat_script_inst)) {
            f32 engaged = 0.0f;

            chuck::vm::vm_thread* thread = script::create_thread(custom_camera_engaged_func_id, inst, get_actor());

            script::push_value(thread, delta_t);
            script::push_value(thread, current_targets->array);
            script::push_value(thread, combat_anim_values->array);
            script::run_thread_with_return(thread, &engaged);

            disengaged = engaged < 0.5f;
        }

        if (disengaged) {
            if (in_custom_cam)
                signal_flags |= combat_signal_animated_camera_off;
        } else if (!in_custom_cam)
            signal_flags |= combat_signal_animated_camera_on;
    }

    bool hero_involved = false;

    if (get_actor()->is_hero())
        hero_involved = true;
    else if (current_targets->size())
        for (auto it = current_targets->begin(); it != current_targets->end(); ++it) {
            auto* target = (entity_base*)(*it).resolve();

            if (target && target->is_hero()) {
                hero_involved = true;

                break;
            }
        }

    if (signal_flags & combat_signal_animated_camera_on) {
        if (hero_involved)
            enter_custom_camera(combo_move);
    } else if (signal_flags & combat_signal_animated_camera_off &&
               (custom_camera_func_id == -1 || custom_camera_engaged_func_id != -1))

        exit_custom_camera();

    if (!retail::sub_97AEF0((u32*)references::game.read())) // game::is_animated_cam_enabled
        in_custom_cam = false;

    // the script drives the animated camera, which is externally controlled only through the call
    if (in_custom_cam && custom_camera_func_id != -1)
        if (chuck::vm::script_instance* inst = script::get_instance(combat_script_inst)) {
            if (combo_move && combo_move->flags_3 & combo_flag_animated_camera) {
                references::g_world_ptr.read()->camera_mgr.animated_camera->flags &= ~entity_flag_externally_controlled;
                references::g_world_ptr.read()->camera_mgr.animated_camera->frame_advance(delta_t);
            }

            references::g_world_ptr.read()->camera_mgr.animated_camera->flags |= entity_flag_externally_controlled;

            chuck::vm::vm_thread* thread = script::create_thread(custom_camera_func_id, inst, get_actor());

            script::push_entity(thread, references::g_world_ptr.read()->camera_mgr.animated_camera);
            script::push_value(thread, delta_t);
            script::push_value(thread, current_targets->array);
            script::push_value(thread, combat_anim_values->array);
            script::run_thread(thread);

            return;
        }

    references::g_world_ptr.read()->camera_mgr.animated_camera->flags &= ~entity_flag_externally_controlled;
}

// sub_4176A0
void combat_state::advance_combat_script(      f32               delta_t,
                                         const combo_move_data*  combo_move,
                                               als_inode*        my_als,
                                               cpu_combat_inode* cmbt) {

    chuck::vm::script_instance* inst = script::get_instance(combat_script_inst);

    if (!inst)
        return;

    if (init_move_func_id != -1) {
        chuck::vm::vm_thread* thread = script::create_thread(init_move_func_id, inst, get_actor());

        script::push_value(thread, current_targets->array);
        script::push_value(thread, combat_anim_values->array);
        script::run_thread(thread);

        init_move_func_id = -1;
    }

    if (frame_advance_func_id != -1) {
        chuck::vm::vm_thread* thread = script::create_thread(frame_advance_func_id, inst, get_actor());

        script::push_value(thread, delta_t);
        script::push_value(thread, current_targets->array);
        script::push_value(thread, combat_anim_values->array);

        auto* info = new combat_thread_info;

        info->the_instance                     = inst;
        info->frame_advancing_combat_state     = this;
        info->frame_advancing_combo_move       = combo_move;
        info->frame_advancing_chain_size       = chain_size;
        info->frame_advancing_cpu_combat_inode = cmbt;
        info->frame_advancing_weapon           = nullptr;
        info->frame_advancing_weapon_entity    = nullptr;
        info->frame_advancing_usage_type       = 4;

        add_combat_thread_info(info);
        script::run_thread(thread);
        remove_combat_thread_info(info);
    }

    f32 speed;

    if (auto* layer = (als::state_machine*)my_als->my_als->get_als_layer(als::base_layer))
        speed = layer->get_playback_speed();
    else
        speed = 1.0f;

    if (playback_speed_func_id != -1) {
        chuck::vm::vm_thread* thread = script::create_thread(playback_speed_func_id, inst, get_actor());

        script::push_value(thread, speed);
        script::push_value(thread, combo_move ? combo_move->get_script_move_id(nullptr) : -1.0f);
        script::push_value(thread, current_targets->array);
        script::push_value(thread, combat_anim_values->array);
        script::run_thread_with_return(thread, &speed);
    }

    if (get_actor()->has_ifc(entity_ifc_player))
        speed = get_actor()->player_ifc()->unk_318() * speed;

    f32 scaled = 1.0f * speed;

    if (als::state_machine_interface* layer = my_als->my_als->get_als_layer(als::base_layer))
        layer->set_speed(scaled);
}

// sub_412A40
void combat_state::apply_hit_dilation_preset(i32 preset, i32 mode) {
    combat_dilation_info info;

    switch (preset) {
        case 1:
            info.duration = 0.125f;
            info.dilation = 0.15f;
            break;

        case 2:
            info.duration = 0.2f;
            info.dilation = 0.15f;
            break;

        case 3:
            info.duration = 0.25f;
            info.dilation = 0.15f;
            break;

        case 4:
            info.duration = 0.7f;
            info.dilation = 0.7f;
            break;

        case 5:
            info.duration = 0.5f;
            info.dilation = 0.5f;
            break;

        case 6:
            info.duration = 0.25f;
            info.dilation = 1.0f;
            break;
    }

    switch (mode) {
        case 0: // the world
            ((time_interface*)retail::sub_968660((u32*)&references::g_world_ptr.read()->time_mgr))->set_combat_dilation(info); // wds_time_manager: the world's time interface
            break;

        case 1: // the actor
            if (!get_actor()->has_ifc(entity_ifc_time))
                get_actor()->create_time_ifc();

            get_actor()->time_ifc()->set_combat_dilation(info);
            break;

        case 2: // the targets
            if (current_targets && current_targets->size() > 0)
                for (auto it = current_targets->begin(); it != current_targets->end(); ++it) {
                    auto* target = (entity_base*)(*it).resolve();

                    if (!target)
                        continue;

                    if (!target->has_ifc(entity_ifc_time))
                        target->create_time_ifc();

                    target->time_ifc()->set_combat_dilation(info);
                }

            break;
    }
}

// sub_409E50
void combat_state::spawn_attack_sound(      f32               delta_t,
                                      const combo_move_data*  combo_move,
                                            als_inode*        my_als,
                                            cpu_combat_inode* cmbt) {

    if (!combo_move || attack_sound_spawned)
        return;

    string_hash sound_grp = combo_move->get_attack_sound_group(nullptr);

    string_hash none;
    none.initialize(mash::ALLOCATED);

    if (sound_grp == none)
        sound_grp = attack_key;

    spawn_sound(combo_move,
                get_actor(),
                sound_grp,
                &get_actor()->my_abs_po->get_position(),
                spawn_attack_sound_func_id,
                &unk_098);

    attack_sound_spawned = true;
}

// sub_412CE0
void combat_state::share_attack_flags_with_hero(      f32               delta_t,
                                                const combo_move_data*  combo_move,
                                                      als_inode*        my_als,
                                                      cpu_combat_inode* cmbt) {

    if (!combo_move || get_actor()->has_ifc(entity_ifc_player) || !current_targets->size())
        return;

    arch_base_vhandle first_target = (*current_targets)[0];

    if (first_target.null())
        return;

    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    if (first_target.resolve() != hero)
        return;

    player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

    player->unk_167((combo_move->unk_14 & combo_unk_14_flag_04000000) != 0);
    player->unk_169((combo_move->unk_14 & combo_unk_14_flag_02000000) != 0);
    player->unk_171((combo_move->unk_14 & combo_unk_14_flag_01000000) != 0);
    player->unk_173((combo_move->unk_14 & combo_unk_14_flag_00800000) != 0);
    player->unk_175((combo_move->unk_14 & combo_unk_14_flag_00000400) != 0);
    player->unk_177((combo_move->unk_18 & combo_unk_18_flag_00000001) != 0);
    player->unk_179((combo_move->unk_18 & combo_unk_18_flag_00000002) != 0);
}

// sub_40A9A0
void combat_state::probe_wall_ahead(      f32               delta_t,
                                    const combo_move_data*  combo_move,
                                          als_inode*        my_als,
                                          cpu_combat_inode* cmbt) {

    if (!combo_move || !(combo_move->unk_10 & combo_unk_10_flag_00800000) || !get_actor()->has_ifc(entity_ifc_physical))
        return;

    vector3d position = get_actor()->my_abs_po->get_position();
    vector3d facing   = get_actor()->my_abs_po->get_z_facing();
    vector3d up       = get_actor()->my_abs_po->get_y_facing();

    als::animation_logic_system_interface* the_als = my_als->my_als;

    f32 height = ((als::state_machine*)the_als->get_als_layer(als::base_layer))->get_float_param((als::animation_logic_system*)the_als,
                                                                                                 als::param_height_above_ground);

    // the line starts at least a meter above the ground
    vector3d start = position;

    if (1.0f > height) {
        f32 raise = (f32)(1.0 - (f64)height);

        const vector3d &y = references::unk_00e6049c.get();

        start = vector3d((f32)((f64)(f32)((f64)raise * y.x) + position.x),
                         (f32)((f64)(f32)((f64)raise * y.y) + position.y),
                         (f32)((f64)(f32)((f64)raise * y.z) + position.z));
    }

    vector3d forward ((f32)((f64)facing.x *  3.0), (f32)((f64)facing.y *  3.0), (f32)((f64)facing.z *  3.0));
    vector3d backward((f32)((f64)facing.x * -1.0), (f32)((f64)facing.y * -1.0), (f32)((f64)facing.z * -1.0));

    vector3d ahead ((f32)((f64)forward.x  + start.x), (f32)((f64)forward.y  + start.y), (f32)((f64)forward.z  + start.z));
    vector3d behind((f32)((f64)backward.x + start.x), (f32)((f64)backward.y + start.y), (f32)((f64)backward.z + start.z));

    line_info probe(behind, ahead);
    retail::sub_798F50((char*)&probe, (i32)references::entfilter_entity_no_capsules.read(), (i32)references::obbfilter_lineseg_test.read(), nullptr, 0); // line_info::check_collision

    f32 facing_wall = (f32)(up * probe.hit_norm);

    if (probe.collision && std::fabs(facing_wall) < 0.1f) {
        retail::sub_4029A0((u32**)my_als, als::param_had_wall_collision,     1.0f,                                     0); // als_inode::set_desired_param
        retail::sub_4029D0((u32**)my_als, als::param_desired_forward_vector, (i32)&probe.hit_norm,                     0);
        retail::sub_4029D0((u32**)my_als, als::param_desired_destination,    (i32)&probe.hit_pos,                      0);
        retail::sub_4029D0((u32**)my_als, als::param_desired_up_vector,      (i32)&references::unk_00e6049c.get(), 0);
    } else
        my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_had_wall_collision, 0.0f);

    retail::sub_798F10((u8*)&probe); // line_info::~line_info
}

// sub_417220
void combat_state::update_v_look(f32 delta_t) {
    auto* my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);

    arch_base_vhandle first_target = current_targets->size() ? (*current_targets)[0] : arch_base_vhandle();
    auto*             target       = (entity_base*)first_target.resolve();

    // the first target's height as an angle off the actor's up, 0 above and 1 below
    f32 look = 0.5f;

    if (target) {
        const vector3d &from = get_actor()->my_abs_po->get_position();
        const vector3d &to   = target->my_abs_po->get_position();

        vector3d dir((f32)((f64)to.x - from.x), (f32)((f64)to.y - from.y), (f32)((f64)to.z - from.z));

        if (dir.length2() > 0.0001f)
            dir.normalize();
        else
            dir = get_actor()->my_abs_po->get_z_facing();

        if (my_als->get_optional_als_pb_int(meta_type_v_look_key, 0, als::base_layer, nullptr)) {
            f32 dot     = (f32)(get_actor()->my_abs_po->get_y_facing() * dir);
            f32 from_up = (f32)(1.570796370506287 - math::asin(dot));
            f32 degrees = (f32)(from_up * 57.29577791868205);

            if (degrees >= 0.0f && 180.0f >= degrees)
                look = (f32)((f64)degrees / 180.0);
        }
    }

    f32 rate_delta = my_als->get_optional_als_pb_float(meta_type_v_look_rate_delta_key, 70.0f, als::base_layer, nullptr);
    f32 rate_damp  = my_als->get_optional_als_pb_float(meta_type_v_look_rate_damp_key,  10.0f, als::base_layer, nullptr);
    f32 rate_mute  = my_als->get_optional_als_pb_float(meta_type_v_look_rate_mute_key,  10.0f, als::base_layer, nullptr);

    v_look.stiffness = rate_delta;
    v_look.damping   = rate_damp;
    v_look.mute_rate = rate_mute;
    v_look.update(delta_t, look);

    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_meta_linear_blend_amount_5, v_look.value);
}

// sub_417000
void combat_state::update_time_dilation(als_inode* my_als) {
    if (time_dilation_start_time == time_dilation_end_time)
        time_dilation_scale = time_dilation_end_scale;
    else {
        f32 duration   = my_als->get_duration(als::base_layer);
        f32 normalized = my_als->get_current_normalized_time(als::base_layer);

        f32 anim_time = (f32)(((f64)normalized - (f32)(i32)normalized) * duration);

        if (anim_time >= time_dilation_end_time)
            time_dilation_scale = time_dilation_end_scale;
        else if (anim_time >= time_dilation_start_time)
            time_dilation_scale = (f32)(((f64)time_dilation_end_scale - time_dilation_start_scale) *
                                        ((f64)anim_time - time_dilation_start_time)                /
                                        ((f64)time_dilation_end_time - time_dilation_start_time)   +
                                        time_dilation_start_scale);
    }

    switch (time_dilation_mode) {
        case 0: // the world
            ((time_interface*)retail::sub_968660((u32*)&references::g_world_ptr.read()->time_mgr))->set_attack_dilation(time_dilation_scale); // wds_time_manager: the world's time interface
            break;

        case 1: // the actor
            if (!get_actor()->has_ifc(entity_ifc_time))
                get_actor()->create_time_ifc();

            get_actor()->time_ifc()->set_attack_dilation(time_dilation_scale);
            break;

        case 2: // the targets
            if (current_targets && current_targets->size() > 0)
                for (auto it = current_targets->begin(); it != current_targets->end(); ++it) {
                    auto* target = (entity_base*)(*it).resolve();

                    if (!target)
                        continue;

                    if (!target->has_ifc(entity_ifc_time))
                        target->create_time_ifc();

                    target->time_ifc()->set_attack_dilation(time_dilation_scale);
                }

            break;
    }
}

// sub_41A430
void combat_state::auto_calc_throw_target(const combo_move_data* combo_move, als_inode* my_als) {
    throw_target = arch_base_vhandle();

    f32 radius    = my_als->get_optional_als_pb_float(auto_calc_throw_target_radius_key, 20.0f, als::base_layer, nullptr);
    f32 min_range = my_als->get_optional_als_pb_float(auto_calc_throw_target_radius_key, 0.0f,  als::base_layer, nullptr); // the radius key again
    f32 arc       = my_als->get_optional_als_pb_float(auto_calc_throw_target_arc_key,    60.0f, als::base_layer, nullptr);

    vector3d position = get_actor()->my_abs_po->get_position();
    vector3d facing;

    if (get_actor()->has_ifc(entity_ifc_player))
        facing = get_actor()->player_ifc()->player_axis[0]->get_player_dir();
    else {
        const matrix4x4 &m = get_actor()->my_abs_po->get_matrix();

        facing = vector3d((f32)((f64)m.y.x * 0.0 + (f64)m.x.x * 0.0 + m.z.x + (f64)m.w.x * 0.0),
                          (f32)((f64)m.y.y * 0.0 + (f64)m.x.y * 0.0 + m.z.y + (f64)m.w.y * 0.0),
                          (f32)((f64)m.y.z * 0.0 + (f64)m.x.z * 0.0 + m.z.z + (f64)m.w.z * 0.0));
    }

    vector3d dir = math::unitize(facing);

    f32 min_dot = math::cos((f32)((f64)arc * 0.5 * 0.017453293005625408));

    vc8::vector<arch_base_vhandle> excluded;

    for (arch_base_vhandle target : *current_targets)
        excluded.push_back(target);

    vector3d up(0.0f, 1.0f, 0.0f);

    auto find_best_damageable_target = (entity_base* (__cdecl*)(entity_base*,
                                                                const vector3d*,
                                                                const vector3d*,
                                                                const vector3d*,
                                                                f32,
                                                                f32,
                                                                f32,
                                                                f32,
                                                                f32,
                                                                f32,
                                                                ai_target::accept_func,
                                                                ai_target*,
                                                                vc8::vector<arch_base_vhandle>*,
                                                                bool,
                                                                u32))retail::sub_4714F0;

    entity_base* best = find_best_damageable_target(get_actor(),
                                                    &position,
                                                    &up,
                                                    &dir,
                                                    min_range,
                                                    radius,
                                                    -0.0f - radius,
                                                    radius,
                                                    min_dot,
                                                    -1.1f,
                                                    nullptr,
                                                    nullptr,
                                                    &excluded,
                                                    true,
                                                    ai_target::target_ai_enemy      |
                                                    ai_target::target_damageable    |
                                                    ai_target::target_alive         |
                                                    ai_target::target_los           |
                                                    ai_target::target_beam_los      |
                                                    ai_target::target_active_switch |
                                                    ai_target::target_alarm_switch  |
                                                    ai_target::target_unk_00040000  |
                                                    ai_target::target_unk_04000000);

    if (best)
        throw_target = best->my_handle;
}

i32 combat_state::search_area_targets(const combo_move_data* combo_move, als_inode* my_als, vc8::vector<ai_target>* targets) {
    return ((i32 (__thiscall*)(combat_state*, const combo_move_data*, als_inode*, vc8::vector<ai_target>*))retail::sub_41B700)(this,
                                                                                                                               combo_move,
                                                                                                                               my_als,
                                                                                                                               targets);
}

void combat_state::update_hit_sphere_positions(als_inode* my_als) {
    ((void (__thiscall*)(combat_state*, als_inode*))retail::sub_40CB10)(this, my_als);
}

// sub_409F20
void combat_state::save_prev_hit_sphere_positions(als_inode* my_als) {
    for (u32 sphere = 0; sphere < my_als->my_als->get_als_layer(als::base_layer)->get_num_unk_44(); ++sphere) {
        hit_sphere_prev_pos[sphere]   = hit_sphere_pos[sphere];
        hit_sphere_prev_valid[sphere] = true;
    }
}

// sub_40A050
void combat_state::wait_for_target_list_lock() {
    engine_lock_scope lock(&references::target_list_lock.get());
}

resource_key combat_state::get_sync_react_asg(entity* target) const {
    resource_key graph;

    retail::sub_40A070((void*)this, (u32*)&graph, target);

    return graph;
}

// sub_40F020
void combat_state::release_sync_target(actor* target) {
    get_actor();

    if (!target)
        return;

    if (target->get_als() && target->anim_ctrl)
        retail::sub_5F5E90(target->anim_ctrl, nullptr, nullptr); // animation_controller::set_master

    if (target->has_ifc(entity_ifc_standing) && target->standing_ifc()->is_enabled())
        target->standing_ifc()->manage_standing(0.01f, true, 0.5f);

    if (ai_core* target_core = target->get_ai_core()) {
        i32 wait_result = 0;

        if (ai_core::wait_for_other_ai_retrying(get_core(), target_core, 10, &wait_result) || wait_result == 2) {
            target_core->pop_base_machine(get_sync_react_asg(target), true);

            if (wait_result == 1)
                retail::sub_4CC650((i32)get_core(), (u32*)target_core); // release_wait_for_other_ai
        }
    }

    target->raise_event(references::ai_combat_sync_end.read());
}

// sub_40F130
arch_base_vhandle combat_state::engage_sync_target(arch_base_vhandle target) {
    arch_base_vhandle engaged;

    auto* target_actor = (actor*)target.resolve();

    if (!target_actor || !target_actor->is_an_actor() || !target_actor->get_als() || in_sync_combo_anim)
        return engaged;

    animation_controller* master_ctrl = get_actor()->anim_ctrl;
    ai_core*              target_core = target_actor->get_ai_core();

    if (target_core && !target_core->attempt_push_base_machine(get_core(), get_sync_react_asg(target_actor), true))
        return engaged;

    engaged = target;

    target_actor->raise_event(references::ai_combat_sync_begin.read());

    if (target_core) {
        auto* car = (ai_car_inode*)target_core->get_info_node(info_node_type_ai_car);

        if (car && car->is_in_car)
            car->force_exit_car(false);
    }

    if (animation_controller* target_ctrl = target_actor->anim_ctrl)
        retail::sub_5F5E90(target_ctrl, (u32**)master_ctrl, nullptr); // animation_controller::set_master

    return engaged;
}

// sub_41F3E0
bool combat_state::engage_sync_targets() {
    auto* als_node = (als_inode*)get_core()->get_info_node(info_node_type_als);

    bool use_multiple = als_node->get_optional_als_pb_int(use_multiple_sync_targets_key, 0, als::base_layer, nullptr) != 0;

    if (sync_targets->size() && (*current_targets)[0] != (*sync_targets)[0])
        kill_sync_anim(false);

    if (!sync_targets->size()) {
        chuck::vm::chuck_dynamic_array_t* targets = current_targets->array;

        for (auto *element = targets->begin(), *end = targets->end(); element != end; ++element) {
            auto &target = *(arch_base_vhandle*)element;

            if (!target.resolve())
                continue;

            // one already running the react machine is left alone
            if (target.resolve()->is_an_actor()) {
                auto* reacting = (actor*)target.resolve();

                if (reacting->get_ai_core() &&
                    reacting->get_ai_core()->find_machine(get_sync_react_asg((actor*)target.resolve())))

                    continue;
            }

            arch_base_vhandle engaged = engage_sync_target(target);

            if (!engaged.null())
                sync_targets->push_back(engaged);

            if (!use_multiple)
                break;
        }
    }

    return sync_targets->size() != 0;
}

// inlined @ sub_41A060
// inlined @ sub_40F230
void combat_state::update_initially_standing() {
    if (!get_actor()->has_ifc(entity_ifc_standing) || !get_actor()->standing_ifc()->is_enabled())
        return;

    get_actor()->standing_ifc()->manage_standing(0.01f, true, 0.5f);

    initially_standing = get_actor()->standing_ifc()->is_effectively_standing();

    if (!initially_standing && is_floor_within_reach())
        initially_standing = true;
}

// inlined @ sub_41F860
// inlined @ sub_41A060
// inlined @ sub_40F230
bool combat_state::is_floor_within_reach() {
    vector3d floor_point;
    vector3d floor_normal;

    if (!retail::sub_42EF70(get_actor(), (f32*)&floor_point, (f32*)&floor_normal, 0))
        return false;

    const vector3d &position = get_actor()->my_abs_po->get_position();

    f32 drop = position.y - floor_point.y;

    return (f32)(get_actor()->standing_ifc()->get_floor_offset() + 0.5) > drop;
}
