#include "retail.hh"
#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/math/references.hh"

using namespace treyarch;

// ==== combo_move_data ==== //

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

i32 combo_move_data::get_combo_move_type_id(bool* exists) const {
    return ((i32 (__thiscall*)(const combo_move_data*, bool*))retail::sub_548530)(this, exists);
}

f32 combo_move_data::get_combo_counter_value(bool* exists) const {
    return (f32)((f64 (__thiscall*)(const combo_move_data*, bool*))retail::sub_4025F0)(this, exists);
}

f32 combo_move_data::get_combo_counter_delay(bool* exists) const {
    return (f32)((f64 (__thiscall*)(const combo_move_data*, bool*))retail::sub_402620)(this, exists);
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

// slot 11
e_state_exit_code combat_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(combat_state*, f32))retail::sub_41F860)(this, delta_t);
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
    i32   none     = 0;

    bool use_multiple = retail::sub_44F510((u32**)als_node, // als_inode::get_optional_pb_int
                                           (u32*)&use_multiple_sync_targets_key,
                                           (i32)&none,
                                           0,
                                           0) != 0;

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

    if (initially_standing)
        return;

    // still standing while the floor is within reach below
    vector3d floor_point;
    vector3d floor_normal;

    if (!retail::sub_42EF70(get_actor(), (f32*)&floor_point, (f32*)&floor_normal, 0))
        return;

    const vector3d &position = get_actor()->my_abs_po->get_position();

    f32 drop = position.y - floor_point.y;

    if ((f32)(get_actor()->standing_ifc()->get_floor_offset() + 0.5) > drop)
        initially_standing = true;
}
