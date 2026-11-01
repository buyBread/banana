#include "retail.hh"
#include "game/wds/ai/states/plr_combat_states.hh"
#include "game/wds/ai/states/plr_loco_wall_sprint_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_team.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/occupant_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/spiderman_rvb_player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/input/game_button.hh"
#include "game/game_meter_manager.hh"
#include "game/math/po.hh"

using namespace treyarch;

// ==== plr_combat_state ==== //

// slot 1
void plr_combat_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_combat_state*))retail::sub_5488E0)(this);
}

// slot 2
void plr_combat_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_combat_state*))retail::sub_54BB40)(this);
}

// slot 3
void plr_combat_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_combat_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_4E2270)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key plr_combat_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_combat_state*))retail::sub_4D4110)(this);
}

// slot 6
bool plr_combat_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_combat_state*, mash::virtual_types_key))retail::sub_4D4120)(this, parent_class);
}

// sub_4D4160
i32 plr_combat_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_combat_state::activate(      ai_state_machine* the_state_machine,
                                const mashed_state*     the_mashed_state,
                                const mashed_state*     prev_mashed_state,
                                const param_block*      extra_params) {

    ((void (__thiscall*)(plr_combat_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_54C000)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_combat_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_combat_state*, const mashed_state*))retail::sub_54C1A0)(this, next_mashed_state);
}

// sub_57D110
e_state_exit_code plr_combat_state::frame_advance(f32 delta_t) {
    e_state_exit_code the_exit_code = combat_state::frame_advance(delta_t);

    player_interface* player = get_actor()->player_ifc();
    auto*             my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);

    retail::sub_548910((i32)get_actor(), (i32)my_als);

    const combo_move_data* combo_move = my_als->get_combo_move(als::base_layer);

    if (combo_move) {
        player->unk_167((combo_move->unk_14 & combo_unk_14_flag_04000000) != 0);
        player->unk_169((combo_move->unk_14 & combo_unk_14_flag_02000000) != 0);
        player->unk_171((combo_move->unk_14 & combo_unk_14_flag_01000000) != 0);
        player->unk_173((combo_move->unk_14 & combo_unk_14_flag_00800000) != 0);
        player->unk_175((combo_move->unk_14 & combo_unk_14_flag_00000400) != 0);
        player->unk_177((combo_move->unk_18 & combo_unk_18_flag_00000001) != 0);
        player->unk_179((combo_move->unk_18 & combo_unk_18_flag_00000002) != 0);

        if (combo_move->unk_14 & combo_unk_14_flag_00000040)
            player->unk_492(1);

        if (combo_move->unk_14 & combo_unk_14_flag_00000200)
            player->unk_506(1);

        if (combo_move->unk_14 & combo_unk_14_flag_00010000)
            player->unk_512(1);
    }

    player->unk_060(current_targets->size() > 0 ? (*current_targets)[0] : arch_base_vhandle());

    if (combo_move                                                       &&
        (combo_move->flags_1 & combo_flag_allow_flight_thrusters)        &&
        (attack_applied || !current_targets || current_targets->empty()) &&
        retail::sub_601C50((u32*)get_actor())                            &&
        get_actor()->has_standing_ifc()                                  &&
        !get_actor()->standing_ifc()->is_effectively_standing()) {

        entity* nearest = nullptr;

        if (player->unk_077() != 1)
            nearest = (entity*)retail::sub_4B6420((const f32*)&get_actor()->my_abs_po->get_position(), 10.0f, 0.0f);

        player->unk_206(delta_t, nearest, 0);
    }

    if (button_held)
        button_held = attack_button && attack_button->pressed();

    if (the_exit_code != state_exit_code_no_exit && the_exit_code != state_exit_code_damaged) {
        if (player->unk_485()                           &&
            can_handle_exit_code(state_exit_code_crawl) &&
            the_exit_code != state_exit_code_crawl      &&
            player->unk_264(delta_t, nullptr, 0, 0, 0))

            the_exit_code = state_exit_code_crawl;

        player->unk_486(false, 0.0f);
    }

    if (the_exit_code == state_exit_code_no_exit) {
        if (can_handle_exit_code(state_exit_code_zip) &&
            !player->unk_080().null()                 &&
            player->unk_080().resolve()               &&
            player->unk_391(1, 0)) {

            player->unk_158(10);
            the_exit_code = state_exit_code_zip;
        } else {
            for (i32 index = 0; index < 2; ++index) {
                auto* record = (spiderman_rvb_unk_0638*)player->unk_399(index);

                if (record->unk_00) {
                    player->unk_368(delta_t, record);
                    break;
                }
            }

            if (player->unk_0251) {
                if (player->get_command_button(3)->released())
                    the_exit_code = state_exit_code_swing;

                i32 hand = player->unk_395();

                if (hand != -1) {
                    static const string_hash swing_hand_key     = string_hash(hash::djb2("swing_hand"));
                    static const string_hash swing_hand2_key    = string_hash(hash::djb2("swing_hand2"));
                    static const string_hash refire_on_swap_key = string_hash(hash::djb2("refire_on_swap"));

                    i32 swing_hand = my_als->get_optional_als_pb_int(swing_hand_key, hand, als::base_layer, nullptr);

                    bool signal_exists = false;
                    bool refire        = false;

                    f32 time_to_shoot = my_als->get_time_to_signal(references::web_shoot.read(), als::base_layer, &signal_exists, true, nullptr);

                    if (signal_exists && 0.0f > time_to_shoot) {
                        swing_hand = my_als->get_optional_als_pb_int(swing_hand2_key, swing_hand, als::base_layer, nullptr);

                        if (swing_hand != hand)
                            refire = my_als->get_optional_als_pb_int(refire_on_swap_key, 0, als::base_layer, nullptr) != 0;
                    }

                    if (swing_hand != hand)
                        player->unk_129(refire);
                }
            }
        }

        static const string_hash should_stick_to_wall_key = string_hash(hash::djb2("should_stick_to_wall"));

        i32 stick_to_wall = my_als->get_optional_als_pb_int(should_stick_to_wall_key, -1, als::base_layer, nullptr);

        if (stick_to_wall == 1)
            player->unk_486(true, 0.0f);
        else if (stick_to_wall == 0)
            player->unk_486(false, 0.0f);
    }

    return the_exit_code == state_exit_code_failure ? state_exit_code_fall : the_exit_code;
}

// slot 12
void plr_combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const plr_combat_state*, info_node_desc_list*))retail::sub_4E36C0)(this, &the_list);
}

// slot 20
void plr_combat_state::update_next_move() {
    ((void (__thiscall*)(plr_combat_state*))retail::sub_57B440)(this);
}

// slot 21
e_combat_move plr_combat_state::get_next_move(f32 delta_t, i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(plr_combat_state*, f32, i32*))retail::sub_54C460)(this, delta_t, next_script_move_num);
}

// slot 22
e_combat_move plr_combat_state::get_initial_move(i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(plr_combat_state*, i32*))retail::sub_54C410)(this, next_script_move_num);
}

// slot 23
void plr_combat_state::move_transition(const combo_move_data* new_combo_move, u32 unk) {
    ((void (__thiscall*)(plr_combat_state*, const combo_move_data*, u32))retail::sub_54C4B0)(this, new_combo_move, unk);
}

// slot 24
void plr_combat_state::add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(plr_combat_state*, als_inode*, const combo_move_data*))retail::sub_57D7B0)(this, my_als, combo_move);
}

// slot 29
void plr_combat_state::fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) {
    ((void (__thiscall*)(plr_combat_state*, const combo_move_data*, als_inode*, f32))retail::sub_574180)(this, combo_move, my_als, delta_t);
}

// slot 32
void plr_combat_state::clear_targets() {
    ((void (__thiscall*)(plr_combat_state*))retail::sub_5489F0)(this);
}

// slot 33
i32 plr_combat_state::get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const {
    return ((i32 (__thiscall*)(const plr_combat_state*, const combo_move_data*, arch_base_vhandle))retail::sub_5489C0)(this, combo_move, target);
}

// slot 34
i32 plr_combat_state::get_damage(const combo_move_data* combo_move, arch_base_vhandle target) const {
    return ((i32 (__thiscall*)(const plr_combat_state*, const combo_move_data*, arch_base_vhandle))retail::sub_54C760)(this, combo_move, target);
}

// slot 35
vector3d plr_combat_state::unk_035(const combo_move_data* combo_move, arch_base_vhandle target) {
    vector3d result;

    ((vector3d* (__thiscall*)(plr_combat_state*, vector3d*, const combo_move_data*, arch_base_vhandle))retail::sub_5489D0)(this,
                                                                                                                           &result,
                                                                                                                           combo_move,
                                                                                                                           target);

    return result;
}

// slot 48
bool plr_combat_state::unk_048(const combo_move_data* combo_move, void* unk) {
    return ((bool (__thiscall*)(plr_combat_state*, const combo_move_data*, void*))retail::sub_582ED0)(this, combo_move, unk);
}

// sub_54C7B0
bool plr_combat_state::hit_target(      entity*           target,
                                  const combo_move_data*  combo_move,
                                        als_inode*        my_als,
                                        cpu_combat_inode* cmbt,
                                        vector3d*         connect_point) {

    bool              alive_before = target->is_alive();
    player_interface* player       = get_actor()->player_ifc();

    target = player->unk_273(target);

    bool hit     = combat_state::hit_target(target, combo_move, my_als, cmbt, connect_point);
    auto my_team = (ai_team::e_team)retail::sub_42B120((i32)get_core());

    // a friendly ai hit with such a move is picked up instead
    if ((combo_move->unk_10 & combo_unk_10_flag_00200000) &&
        !retail::sub_992A60((u32**)player)                 &&
        target->is_an_actor()                              &&
        alive_before) {

        if (ai_core* their_core = ((actor*)target)->get_ai_core()) {
            auto their_team = (ai_team::e_team)retail::sub_42B120((i32)their_core);

            if (my_team != ai_team::num_teams && their_team != ai_team::num_teams && ai_team::is_friend(my_team, their_team)) {
                if (auto* carry = (std_carry_inode*)get_core()->get_info_node(info_node_type_std_carry)) {
                    auto* their_carry = (std_carry_inode*)their_core->get_info_node(info_node_type_std_carry);

                    if (their_carry && their_carry->get_allow_pickup())
                        carry->add_slave(their_carry, true);
                }
            }
        }
    }

    if (target->is_an_actor() && ((actor*)target)->has_base_ai_data() && ((actor*)target)->get_ai_core())
        player->unk_050(target->my_handle);

    if (target->is_an_actor() && target->has_ifc(entity_ifc_damage)) {
        damage_interface* damage = target->damage_ifc();

        if (!attack_blocked                                     &&
            !attack_dodged                                      &&
            !(target->ext_flags & entity_ext_flag_invulnerable) &&
            (damage->hit_points.val > 0 || (damage->flags & damage_ifc_flag_unk_00001000))) {

            i32 combo_move_type_id = combo_move->get_combo_move_type_id(nullptr);

            if (combo_move_type_id == -1)
                combo_move_type_id = 999;

            retail::sub_987520(combo_move_type_id, (f32)damage->dmg_info.damage);
        }

        if (target->is_an_actor() && get_actor()->has_ifc(entity_ifc_combo)) {
            bool counts = target->is_a_vehicle() ?
                retail::sub_610BE0((i32*)target->occupant_ifc(), my_team) :
                ((actor*)target)->has_base_ai_data() && ((actor*)target)->get_ai_core();

            if (counts) {
                combo_interface* combo         = get_actor()->combo_ifc();
                f32              counter_value = combo_move->get_combo_counter_value(nullptr);
                bool             delays        = false;

                if (counter_value > 0.0f) {
                    i32 experience_kind = retail::sub_60ABE0((u32*)damage, (i32)get_actor(), (i32)combo_move);

                    retail::sub_626630((i32)combo, counter_value, experience_kind);

                    delays = true;
                } else if (0.0f > counter_value)
                    delays = true;

                if (delays) {
                    f32 delay = combo_move->get_combo_counter_delay(nullptr);

                    if (0.0f > delay)
                        delay = (f32)((f64)(f32)references::unk_00bb2184.read() / 30.0);

                    retail::sub_601640(combo, delay);
                }

                f32 gained = (f32)damage->dmg_info.unk_38;

                if (gained > 0.0f) {
                    combo->combo_meter = (f32)((f64)(f32)((f64)references::unk_00bb217c.read() * (f64)gained) + (f64)combo->combo_meter);

                    if (!(combo_move->unk_18 & combo_unk_18_flag_00000010))
                        retail::sub_933BB0((i32*)references::game_meter_manager.read(), 0, (f32)((f64)gained * 0.5));
                }
            }
        }
    }

    if (target->is_an_actor() && ((actor*)target)->has_base_ai_data()) {
        if (combo_move->unk_10 & combo_unk_10_flag_00020000)
            get_actor()->raise_event(references::ground_hit.read());

        if (combo_move->unk_10 & combo_unk_10_flag_00008000)
            get_actor()->raise_event(references::wall_hit.read());

        if (combo_move->unk_10 & combo_unk_10_flag_00010000)
            get_actor()->raise_event(references::air_hit.read());
    }

    player->unk_027(0);
    player->unk_043(1);

    return hit;
}

// slot 55
bool plr_combat_state::apply_jump_connect(const combo_move_data* combo_move) {
    return ((bool (__thiscall*)(plr_combat_state*, const combo_move_data*))retail::sub_548990)(this, combo_move);
}

// slot 65
bool plr_combat_state::check_for_block(      entity*                    target,
                                             i32                        chain_size,
                                       const combo_move_data*           combo_move,
                                             hit_react_record*          react,
                                             weapon_interface_extender* weapon,
                                             entity*                    weapon_entity,
                                             i32                        usage_type,
                                             cpu_combat_inode*          cmbt) {

    return ((bool (__thiscall*)(plr_combat_state*,
                                entity*,
                                i32,
                                const combo_move_data*,
                                hit_react_record*,
                                weapon_interface_extender*,
                                entity*,
                                i32,
                                cpu_combat_inode*))retail::sub_548A00)(this,
                                                                       target,
                                                                       chain_size,
                                                                       combo_move,
                                                                       react,
                                                                       weapon,
                                                                       weapon_entity,
                                                                       usage_type,
                                                                       cmbt);
}

// slot 66
void plr_combat_state::unk_066(actor* the_actor) {
    ((void (__thiscall*)(plr_combat_state*, actor*))retail::sub_4D4170)(this, the_actor);
}

// ==== plr_car_combat_attack_state ==== //

// slot 4
mash::virtual_types_key plr_car_combat_attack_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_car_combat_attack_state*))retail::sub_4D4D70)(this);
}

// slot 6
bool plr_car_combat_attack_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_car_combat_attack_state*, mash::virtual_types_key))retail::sub_4D4D80)(this, parent_class);
}

// slot 9
void plr_car_combat_attack_state::activate(      ai_state_machine* the_state_machine,
                                           const mashed_state*     the_mashed_state,
                                           const mashed_state*     prev_mashed_state,
                                           const param_block*      extra_params) {

    ((void (__thiscall*)(plr_car_combat_attack_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_573610)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_car_combat_attack_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_car_combat_attack_state*, const mashed_state*))retail::sub_5737A0)(this, next_mashed_state);
}

// sub_57FCD0
e_state_exit_code plr_car_combat_attack_state::frame_advance(f32 delta_t) { // a collision knocks the hero off
    u32 physical_flags = get_actor()->physical_ifc()->flags;

    if (!(physical_flags & physical_interface_flag_collided) && !(physical_flags & physical_interface_flag_ext_collided))
        return plr_combat_state::frame_advance(delta_t);

    vector3d up     = get_actor()->my_abs_po->get_y_facing();
    vector3d normal = get_actor()->physical_ifc()->last_collision_normal;

    vector3d scaled((f32)((f64)normal.x * 5.0),
                    (f32)((f64)normal.y * 5.0),
                    (f32)((f64)normal.z * 5.0));

    vector3d override_dir((f32)((f64)scaled.x + up.x),
                          (f32)((f64)scaled.y + up.y),
                          (f32)((f64)scaled.z + up.z));

    string_hash wounded_category;
    wounded_category.initialize(mash::ALLOCATED);

    vector3d dir = get_actor()->physical_ifc()->last_collision_normal;

    const vector3d* position = &get_actor()->my_abs_po->get_position();

    get_actor()->damage_ifc()->apply_damage_with_combo_web_damage(get_actor(),
                                                                  nullptr,
                                                                  0,
                                                                  8,
                                                                  4,
                                                                  nullptr,
                                                                  nullptr,
                                                                  position,
                                                                  &dir,
                                                                  0,
                                                                  &wounded_category,
                                                                  &override_dir,
                                                                  num_rb_phys_bones,
                                                                  nullptr,
                                                                  nullptr,
                                                                  false);

    return state_exit_code_exploded;
}

// ==== rhino_ride_state ==== //

// slot 1
void rhino_ride_state::construct_mashed_class() {
    ((void (__thiscall*)(rhino_ride_state*))retail::sub_54BB60)(this);
}

// slot 2
void rhino_ride_state::destruct_mashed_class() {
    ((void (__thiscall*)(rhino_ride_state*))retail::sub_54BB80)(this);
}

// slot 3
void rhino_ride_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(rhino_ride_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_57D0E0)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key rhino_ride_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const rhino_ride_state*))retail::sub_4D4F20)(this);
}

// slot 6
bool rhino_ride_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const rhino_ride_state*, mash::virtual_types_key))retail::sub_4D4D80)(this, parent_class);
}

// sub_4D6FA0
i32 rhino_ride_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void rhino_ride_state::activate(      ai_state_machine* the_state_machine,
                                const mashed_state*     the_mashed_state,
                                const mashed_state*     prev_mashed_state,
                                const param_block*      extra_params) {

    ((void (__thiscall*)(rhino_ride_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_573DB0)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void rhino_ride_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(rhino_ride_state*, const mashed_state*))retail::sub_573F40)(this, next_mashed_state);
}

// sub_573FF0
e_state_exit_code rhino_ride_state::frame_advance(f32 delta_t) {
    e_state_exit_code the_exit_code = enhanced_state::frame_advance(delta_t);

    if (unk_304->get_start_rhino_ride()) {
        start_combat_script(delta_t);

        als::layer_id layer  = get_combat_layer();
        auto*         my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);
        auto*         cmbt   = (cpu_combat_inode*)get_core()->get_info_node(info_node_type_cpu_combat);

        const combo_move_data* combo_move = my_als->get_combo_move(layer);

        if (my_als->my_als->get_als_layer(layer)->did_transition_succeed()) {
            start_sync_anim(combo_move);
            start_buddy_combo(combo_move);
        }

        fill_combat_anim_values(combo_move, my_als, delta_t);
        set_als_position(my_als, combo_move);
        set_als_facing(my_als, combo_move);
        unk_026(my_als, combo_move);
        add_extra_als_params(my_als, combo_move);
        advance_combat_script(delta_t, combo_move, my_als, cmbt);
    }

    if (!unk_304)
        return state_exit_code_failure;

    if (the_exit_code != state_exit_code_no_exit)
        return the_exit_code;

    return unk_304->get_start_rhino_ride() ? the_exit_code : state_exit_code_success;
}

// ==== plr_trick_state ==== //

// slot 1
void plr_trick_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_trick_state*))retail::sub_5488E0)(this);
}

// slot 2
void plr_trick_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_trick_state*))retail::sub_54BB40)(this);
}

// slot 3
void plr_trick_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_trick_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_4E2270)(this,
                                                                                                                  mash_info,
                                                                                                                  containing_class_ptr,
                                                                                                                  buffer);
}

// slot 4
mash::virtual_types_key plr_trick_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_trick_state*))retail::sub_4D6F90)(this);
}

// slot 6
bool plr_trick_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_trick_state*, mash::virtual_types_key))retail::sub_4D4120)(this, parent_class);
}

// sub_4D6FA0
i32 plr_trick_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_trick_state::activate(      ai_state_machine* the_state_machine,
                               const mashed_state*     the_mashed_state,
                               const mashed_state*     prev_mashed_state,
                               const param_block*      extra_params) {

    ((void (__thiscall*)(plr_trick_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_54CC40)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_trick_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_trick_state*, const mashed_state*))retail::sub_54CE50)(this, next_mashed_state);
}

// sub_54CF20
e_state_exit_code plr_trick_state::frame_advance(f32 delta_t) {
    e_state_exit_code the_exit_code = combat_state::frame_advance(delta_t);

    player_interface* player = get_actor()->player_ifc();

    if (button_held)
        button_held = attack_button && attack_button->pressed();

    if (get_actor()->physical_ifc()->drag_coefficient_horz_scale < 1.0f) {
        f32 horz_scale = (f32)((f64)references::unk_00e66f10.read() * delta_t + get_actor()->physical_ifc()->drag_coefficient_horz_scale);

        if (horz_scale < 0.0f)
            horz_scale = 0.0f;
        else if (horz_scale > 1.0f)
            horz_scale = 1.0f;

        get_actor()->physical_ifc()->drag_coefficient_horz_scale = horz_scale;
    }

    if (references::unk_00f4d590.read() > get_actor()->physical_ifc()->gravity_multiplier) {
        f32 gravity_multiplier = (f32)((f64)references::unk_00e66f0c.read() * delta_t + get_actor()->physical_ifc()->gravity_multiplier);

        if (gravity_multiplier < 0.0f)
            gravity_multiplier = 0.0f;
        else if (gravity_multiplier > references::unk_00f4d590.read())
            gravity_multiplier = references::unk_00f4d590.read();

        get_actor()->physical_ifc()->gravity_multiplier = gravity_multiplier;
    }

    unk_314 += delta_t;

    if (the_exit_code == state_exit_code_jump)
        the_exit_code = state_exit_code_fall;

    auto* my_als = (als_inode*)get_core()->get_info_node(info_node_type_als);

    if (const combo_move_data* combo_move = my_als->get_combo_move(als::base_layer))
        retail::sub_987560(combo_move->get_combo_move_type_id(nullptr), 0.0f);

    if (the_exit_code == state_exit_code_no_exit) {
        entity* nearest = nullptr;

        if (player->unk_077() != 1)
            nearest = (entity*)retail::sub_4B6420((const f32*)&get_actor()->my_abs_po->get_position(), 5.0f, 0.0f);

        player->unk_206(delta_t, nearest, 0);

        if (can_handle_exit_code(state_exit_code_wall_sprint) && player->unk_319(delta_t, unk_300, 0))
            return state_exit_code_wall_sprint;

        if (can_handle_exit_code(state_exit_code_crawl) && player->unk_264(delta_t, nullptr, 0, 0, 0))
            return state_exit_code_crawl;

        if (can_handle_exit_code(state_exit_code_pole_swing) && player->unk_392(0))
            return state_exit_code_pole_swing;

        if (can_handle_exit_code(state_exit_code_yoyo) &&
            unk_304                                    &&
            unk_304->triggered()                       &&
            player->unk_384())

            return state_exit_code_yoyo;

        if (can_handle_exit_code(state_exit_code_jump) &&
            player->unk_182()                          &&
            unk_308                                    &&
            unk_308->triggered()                       &&
            !(unk_30c && unk_30c->triggered())) {

            player->unk_158(13);

            return state_exit_code_jump;
        }
    }

    return the_exit_code;
}

// slot 20
void plr_trick_state::update_next_move() {
    ((void (__thiscall*)(plr_trick_state*))retail::sub_54D350)(this);
}

// slot 21
e_combat_move plr_trick_state::get_next_move(f32 delta_t, i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(plr_trick_state*, f32, i32*))retail::sub_54D3E0)(this, delta_t, next_script_move_num);
}

// slot 22
e_combat_move plr_trick_state::get_initial_move(i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(plr_trick_state*, i32*))retail::sub_548A10)(this, next_script_move_num);
}

// slot 23
void plr_trick_state::move_transition(const combo_move_data* new_combo_move, u32 unk) {
    ((void (__thiscall*)(plr_trick_state*, const combo_move_data*, u32))retail::sub_54D400)(this, new_combo_move, unk);
}

// slot 24
void plr_trick_state::add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(plr_trick_state*, als_inode*, const combo_move_data*))retail::sub_548A30)(this, my_als, combo_move);
}

// slot 29
void plr_trick_state::fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) {
    ((void (__thiscall*)(plr_trick_state*, const combo_move_data*, als_inode*, f32))retail::sub_574400)(this, combo_move, my_als, delta_t);
}

// slot 66
void plr_trick_state::unk_066(actor* the_actor) {
    ((void (__thiscall*)(plr_trick_state*, actor*))retail::sub_4D6FB0)(this, the_actor);
}
