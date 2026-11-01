#include "retail.hh"
#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_rappel_inode.hh"
#include "game/wds/ai/cpu_combat_inode.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/script_data_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/als/als.hh"
#include "game/als/state_machine.hh"
#include "game/animation/animation_controller.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "game/script/script_access.hh"
#include "game/colgeom.hh"
#include "game/quest_manager.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"

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
f32 combo_move_data::get_script_move_id(bool* exists) const {
    return params.get_optional_pb_float(script_move_id_key, -1.0f, exists);
}

// sub_409020
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
        !(get_actor()->has_ifc(entity_ifc_damage) && get_actor()->damage_ifc()->flags & damage_ifc_flag_unk_08000000)) {

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

                get_actor()->get_ai_core()->flags &= ~ai_core_flag_unk_00040000;

                if (combo_move && combo_move->unk_18 & combo_unk_18_flag_00008000)
                    unk_1f2 = false;

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
                    unk_220 = 1.0f;
                    unk_214 = 1.0f;
                    unk_210 = F32_MAX;
                    unk_21c = 1.0f;
                    unk_218 = F32_MAX;

                    ((void (__thiscall*)(combat_state*, als_inode*))retail::sub_417000)(this, my_als);
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

                    vc8::vector<auto_calc_target> auto_calc_targets;

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
                    ((void (__thiscall*)(combat_state*, const combo_move_data*, als_inode*))retail::sub_41A430)(this, combo_move, my_als);

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
                                retail::sub_65B290((u32**)get_actor(), (i32)target->my_handle.value); // actor::add_collision_ignorance
                        } else
                            retail::sub_64B1D0((u32*)get_actor(), (i32)target->my_handle.value); // actor::remove_collision_ignorance
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

                unk_1ec = false;

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

            ((void (__thiscall*)(combat_state*, als_inode*))retail::sub_417000)(this, my_als);

            (*combat_anim_values)[50] = next_move == combat_num_moves ? -1.0f : get_combat_move_script_code(next_move, next_script_move_num);

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

            if (!unk_1e2                                        &&
                combo_move                                      &&
                combo_move->flags_3 & combo_flag_3_unk_00000400 &&
                combo_move->unk_18 & combo_unk_18_flag_00000008)

                unk_060(delta_t, combo_move);

            if (signal_flags & combat_signal_anim_action) {
                if (auto_draw_move)
                    perform_auto_draw(combo_move);
                else if (auto_holster_move)
                    perform_auto_holster(combo_move);

                if (!unk_1e2                                           &&
                    combo_move                                         &&
                    combo_move->flags_3 & combo_flag_3_unk_00000400    &&
                    combo_move->get_jump_launch_force(nullptr) != 0.0f &&
                    get_actor()->has_physical_ifc()) {

                    unk_1e2            = unk_056(combo_move);
                    initially_standing = false;
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

                ((void (__thiscall*)(combat_state*, const combo_move_data*, als_inode*))retail::sub_41A430)(this, next_combo_move, my_als);

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

    ((void (__thiscall*)(combat_state*, f32, const combo_move_data*, als_inode*, cpu_combat_inode*))retail::sub_409E50)(this, delta_t, combo_move, my_als, cmbt);
    ((void (__thiscall*)(combat_state*, f32, const combo_move_data*, als_inode*, cpu_combat_inode*))retail::sub_412CE0)(this, delta_t, combo_move, my_als, cmbt);
    ((void (__thiscall*)(combat_state*, f32, const combo_move_data*, als_inode*, cpu_combat_inode*))retail::sub_40A9A0)(this, delta_t, combo_move, my_als, cmbt);
    ((void (__thiscall*)(combat_state*, f32))retail::sub_417220)(this, delta_t);

    signal_flags = 0;

    if (get_actor()->has_ifc(entity_ifc_damage))
        get_actor()->damage_ifc()->flags &= ~damage_ifc_flag_unk_08000000;

    if (cmbt)
        cmbt->set_script_chain_size(chain_size);

    return the_exit_code;
}

// slot 12
void combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const combat_state*, info_node_desc_list*))retail::sub_54B5C0)(this, &the_list);
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

// slot 26
void combat_state::unk_026(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, als_inode*, const combo_move_data*))retail::sub_415470)(this, my_als, combo_move);
}

// slot 27
void combat_state::set_als_position(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, als_inode*, const combo_move_data*))retail::sub_415B20)(this, my_als, combo_move);
}

// slot 28
void combat_state::update_combat(f32               delta_t,
                                 e_combat_move     next_move_enum,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt) {

    ((void (__thiscall*)(combat_state*,
                         f32,
                         e_combat_move,
                         als_inode*,
                         cpu_combat_inode*))retail::sub_41C930)(this,
                                                                delta_t,
                                                                next_move_enum,
                                                                my_als,
                                                                cmbt);
}

// slot 29
void combat_state::fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*, als_inode*, f32))retail::sub_410DB0)(this, combo_move, my_als, delta_t);
}

// slot 30
bool combat_state::is_combo_cancelled(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) const {
    return ((bool (__thiscall*)(const combat_state*, const combo_move_data*, als_inode*, f32))retail::sub_409D90)(this, combo_move, my_als, delta_t);
}

// slot 31
bool combat_state::unk_031(f32 unk) {
    return ((bool (__thiscall*)(combat_state*, f32))retail::sub_40AD50)(this, unk);
}

// slot 32
void combat_state::clear_targets() {
    ((void (__thiscall*)(combat_state*))retail::sub_41A230)(this);
}

// slot 33
i32 combat_state::get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const {
    return ((i32 (__thiscall*)(const combat_state*, const combo_move_data*, arch_base_vhandle))retail::sub_40DDD0)(this, combo_move, target);
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

// slot 37
void combat_state::exit_custom_camera() {
    ((void (__thiscall*)(combat_state*))retail::sub_410CC0)(this);
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
        retail::sub_42EA60((u32*)get_actor(), (i32)&references::unk_00fc1ee0.get(), (i32)&references::unk_00e6049c.get(), false);

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

// slot 44
void combat_state::spawn_sound(const combo_move_data* combo_move,
                                     entity*          player,
                                     string_hash      sound_grp,
                               const vector3d*        connect_point,
                                     i32              sound_func_id,
                                     void*            unk) {

    ((void (__thiscall*)(combat_state*,
                         const combo_move_data*,
                         entity*,
                         string_hash,
                         const vector3d*,
                         i32,
                         void*))retail::sub_40EDC0)(this,
                                                    combo_move,
                                                    player,
                                                    sound_grp,
                                                    connect_point,
                                                    sound_func_id,
                                                    unk);
}

// slot 45
void combat_state::spawn_sound_impact(const combo_move_data* combo_move,
                                            entity*          attacker,
                                            entity*          target,
                                      const vector3d*        connect_point,
                                            i32              damage,
                                            bool             blocked) {

    ((void (__thiscall*)(combat_state*,
                         const combo_move_data*,
                         entity*,
                         entity*,
                         const vector3d*,
                         i32,
                         bool))retail::sub_40EBA0)(this,
                                                   combo_move,
                                                   attacker,
                                                   target,
                                                   connect_point,
                                                   damage,
                                                   blocked);
}

// slot 46
string_hash combat_state::get_combat_als_category() const {
    string_hash category;

    ((string_hash* (__thiscall*)(const combat_state*, string_hash*))retail::sub_409FA0)(this, &category);

    return category;
}

// slot 47
als::layer_id combat_state::get_combat_layer() {
    return ((als::layer_id (__thiscall*)(combat_state*))retail::sub_40A8F0)(this);
}

// sub_5BD2C0
bool combat_state::unk_048(const combo_move_data*, void*) {
    return false;
}

// slot 49
bool combat_state::unk_049(      entity*           target,
                           const combo_move_data*  combo_move,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt,
                                 vector3d*         connect_point) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                const combo_move_data*,
                                als_inode*,
                                cpu_combat_inode*,
                                vector3d*))retail::sub_40BF60)(this,
                                                               target,
                                                               combo_move,
                                                               my_als,
                                                               cmbt,
                                                               connect_point);
}

// slot 50
bool combat_state::unk_050(      entity*           target,
                           const combo_move_data*  combo_move,
                                 als_inode*        my_als,
                                 cpu_combat_inode* cmbt,
                                 vector3d*         connect_point) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                const combo_move_data*,
                                als_inode*,
                                cpu_combat_inode*,
                                vector3d*))retail::sub_40D290)(this,
                                                               target,
                                                               combo_move,
                                                               my_als,
                                                               cmbt,
                                                               connect_point);
}

// slot 51
bool combat_state::unk_051(entity* target, const vector3d* unk, const vector3d* unk_2) {
    return ((bool (__thiscall*)(combat_state*, entity*, const vector3d*, const vector3d*))retail::sub_40DAB0)(this, target, unk, unk_2);
}

// slot 52
bool combat_state::check_for_target_hit(      entity*           target,
                                        const combo_move_data*  combo_move,
                                              als_inode*        my_als,
                                              cpu_combat_inode* cmbt,
                                              vector3d*         connect_point) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                const combo_move_data*,
                                als_inode*,
                                cpu_combat_inode*,
                                vector3d*))retail::sub_40DD30)(this,
                                                               target,
                                                               combo_move,
                                                               my_als,
                                                               cmbt,
                                                               connect_point);
}

// slot 53
bool combat_state::hit_target(      entity*           target,
                              const combo_move_data*  combo_move,
                                    als_inode*        my_als,
                                    cpu_combat_inode* cmbt,
                                    vector3d*         connect_point) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                const combo_move_data*,
                                als_inode*,
                                cpu_combat_inode*,
                                vector3d*))retail::sub_41DD80)(this,
                                                               target,
                                                               combo_move,
                                                               my_als,
                                                               cmbt,
                                                               connect_point);
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

// slot 62
void combat_state::exit_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*, als_inode*, cpu_combat_inode*))retail::sub_418B40)(this, combo_move, my_als, cmbt);
}

// slot 63
void combat_state::unk_063(als_inode* my_als, entity* target) {
    ((void (__thiscall*)(combat_state*, als_inode*, entity*))retail::sub_40F930)(this, my_als, target);
}

// slot 64
bool combat_state::check_for_dodge(      entity*                    target,
                                         i32                        chain_size,
                                   const combo_move_data*           combo_move,
                                         weapon_interface_extender* weapon,
                                         entity*                    weapon_entity,
                                         i32                        usage_type,
                                         cpu_combat_inode*          cmbt) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                i32,
                                const combo_move_data*,
                                weapon_interface_extender*,
                                entity*,
                                i32,
                                cpu_combat_inode*))retail::sub_40E9E0)(this,
                                                                       target,
                                                                       chain_size,
                                                                       combo_move,
                                                                       weapon,
                                                                       weapon_entity,
                                                                       usage_type,
                                                                       cmbt);
}

// slot 65
bool combat_state::check_for_block(      entity*                    target,
                                         i32                        chain_size,
                                   const combo_move_data*           combo_move,
                                         hit_react_record*          react,
                                         weapon_interface_extender* weapon,
                                         entity*                    weapon_entity,
                                         i32                        usage_type,
                                         cpu_combat_inode*          cmbt) {

    return ((bool (__thiscall*)(combat_state*,
                                entity*,
                                i32,
                                const combo_move_data*,
                                hit_react_record*,
                                weapon_interface_extender*,
                                entity*,
                                i32,
                                cpu_combat_inode*))retail::sub_40EAD0)(this,
                                                                       target,
                                                                       chain_size,
                                                                       combo_move,
                                                                       react,
                                                                       weapon,
                                                                       weapon_entity,
                                                                       usage_type,
                                                                       cmbt);
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

void combat_state::kill_combat_script(bool unload_exec) {
    ((void (__thiscall*)(combat_state*, bool))retail::sub_410B90)(this, unload_exec);
}

// sub_41F7B0
void combat_state::push_target(arch_base_vhandle targ) {
    engine_lock_scope lock(&references::target_list_lock.get());

    arch_base* target = targ.resolve();

    if (target && target != get_actor()) {
        current_targets->push_back(targ);

        if (retail::sub_601C50((u32*)target)) // entity_base::is_hero
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

i32 combat_state::internal_auto_calc_targets(const combo_move_data* combo_move, vc8::vector<auto_calc_target>* targets) {
    return ((i32 (__thiscall*)(combat_state*, const combo_move_data*, vc8::vector<auto_calc_target>*))retail::sub_41BF10)(this, combo_move, targets);
}

void combat_state::set_delay_stun_recovery(const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_40A000)(this, combo_move);
}

void combat_state::perform_auto_draw(const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_40EEC0)(this, combo_move);
}

void combat_state::perform_auto_holster(const combo_move_data* combo_move) {
    ((void (__thiscall*)(combat_state*, const combo_move_data*))retail::sub_40EF70)(this, combo_move);
}

void combat_state::update_camera(      f32               delta_t,
                                 const combo_move_data*  combo_move,
                                       als_inode*        my_als,
                                       cpu_combat_inode* cmbt) {

    ((void (__thiscall*)(combat_state*,
                         f32,
                         const combo_move_data*,
                         als_inode*,
                         cpu_combat_inode*))retail::sub_412E30)(this,
                                                                delta_t,
                                                                combo_move,
                                                                my_als,
                                                                cmbt);
}

void combat_state::advance_combat_script(      f32               delta_t,
                                         const combo_move_data*  combo_move,
                                               als_inode*        my_als,
                                               cpu_combat_inode* cmbt) {

    ((void (__thiscall*)(combat_state*,
                         f32,
                         const combo_move_data*,
                         als_inode*,
                         cpu_combat_inode*))retail::sub_4176A0)(this,
                                                                delta_t,
                                                                combo_move,
                                                                my_als,
                                                                cmbt);
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

        if (retail::sub_402A00((i32)get_core(), (i32)target_core, 10, (u32*)&wait_result) || wait_result == 2) {
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
