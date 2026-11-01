#include "retail.hh"
#include "game/wds/ai/states/sync_anim_state.hh"
#include "game/als/param.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// ==== sync_anim_state ==== //

// slot 1
void sync_anim_state::construct_mashed_class() {
    ((void (__thiscall*)(sync_anim_state*))retail::sub_44F180)(this);
}

// slot 2
void sync_anim_state::destruct_mashed_class() {
    ((void (__thiscall*)(sync_anim_state*))retail::sub_44F1A0)(this);
}

// slot 3
void sync_anim_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(sync_anim_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_47FE00)(this,
                                                                                                                  mash_info,
                                                                                                                  containing_class_ptr,
                                                                                                                  buffer);
}

// slot 4
mash::virtual_types_key sync_anim_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const sync_anim_state*))retail::sub_4BF630)(this);
}

// slot 6
bool sync_anim_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const sync_anim_state*, mash::virtual_types_key))retail::sub_4D69F0)(this, parent_class);
}

// sub_4D0C40
i32 sync_anim_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_457C60
void sync_anim_state::activate(      ai_state_machine* the_state_machine,
                               const mashed_state*     the_mashed_state,
                               const mashed_state*     prev_mashed_state,
                               const param_block*      extra_params) {

    attack_enabled_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    preslave_setup();

    engaged_slave_mode     = false;
    master_played_category = string_hash();
    slave_played_category  = string_hash();
    played_layer           = als::base_layer;

    actor* slave_actor  = get_slave();
    actor* master_actor = get_master();

    master = master_actor ? master_actor->my_handle : arch_base_vhandle();
    slave  = slave_actor  ? slave_actor ->my_handle : arch_base_vhandle();

    // whichever of the two isn't this state's actor goes into slave mode
    actor* other = master_actor != get_actor() ? master_actor : slave_actor;

    if (!master_actor || !slave_actor)
        return;

    als::animation_logic_system_interface* master_als = master_actor->get_als();
    als::animation_logic_system_interface* slave_als  = slave_actor ->get_als();

    if (!master_als || !slave_als)
        return;

    if (engage_ai_slave_mode() && other) {
        if (ai_core* other_core = other->get_ai_core()) {
            arch_base_vhandle new_master = get_actor()->my_handle;

            engaged_slave_mode = other_core->attempt_to_engage_slave_mode(new_master, get_desired_als_layer());
        }
    }

    animation_controller* master_ctrl = master_actor->anim_ctrl;

    if (animation_controller* slave_ctrl = slave_actor->anim_ctrl)
        retail::sub_5F5E90(slave_ctrl, (u32**)master_ctrl, (u32**)get_desired_als_layer()); // animation_controller::set_master

    play_anims(get_master_category(), get_slave_category());
}

// slot 10
void sync_anim_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(sync_anim_state*, const mashed_state*))retail::sub_457E20)(this, next_mashed_state);
}

// slot 11
e_state_exit_code sync_anim_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(sync_anim_state*, f32))retail::sub_457F20)(this, delta_t);
}

// slot 12
void sync_anim_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const sync_anim_state*, info_node_desc_list*))retail::sub_54B5C0)(this, &the_list);
}

// slot 24
bool sync_anim_state::unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const {
    return ((bool (__thiscall*)(const sync_anim_state*, als_inode*, als::category_id, als::layer_id))retail::sub_44F6A0)(this, node, cat, layer);
}

// slot 25
als::category_id sync_anim_state::get_master_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const sync_anim_state*, als::category_id*))retail::sub_4BF610)(this, &category);

    return category;
}

// slot 26
als::category_id sync_anim_state::get_slave_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const sync_anim_state*, als::category_id*))retail::sub_48E910)(this, &category);

    return category;
}

// sub_A944D0
als::layer_id sync_anim_state::get_desired_als_layer() const {
    return als::base_layer;
}

// sub_54B2F0
actor* sync_anim_state::get_master() const {
    return get_actor();
}

// sub_667F80
bool sync_anim_state::engage_ai_slave_mode() const {
    return true;
}

// slot 31
void sync_anim_state::play_anims(als::category_id master_cat, als::category_id slave_cat) {
    ((void (__thiscall*)(sync_anim_state*, als::category_id, als::category_id))retail::sub_44F6D0)(this, master_cat, slave_cat);
}

// sub_5B4A70
void sync_anim_state::preslave_setup() {}

// sub_667F80
bool sync_anim_state::unk_033() const {
    return true;
}

// ==== generic_sync_anim_state ==== //

// slot 1
void generic_sync_anim_state::construct_mashed_class() {
    ((void (__thiscall*)(generic_sync_anim_state*))retail::sub_48F5D0)(this);
}

// slot 2
void generic_sync_anim_state::destruct_mashed_class() {
    ((void (__thiscall*)(generic_sync_anim_state*))retail::sub_4E2280)(this);
}

// slot 3
void generic_sync_anim_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(generic_sync_anim_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                          mash_info,
                                                                                                                          containing_class_ptr,
                                                                                                                          buffer);
}

// slot 4
mash::virtual_types_key generic_sync_anim_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const generic_sync_anim_state*))retail::sub_4D0AC0)(this);
}

// slot 6
bool generic_sync_anim_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const generic_sync_anim_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_492650
void generic_sync_anim_state::activate(      ai_state_machine* the_state_machine,
                                       const mashed_state*     the_mashed_state,
                                       const mashed_state*     prev_mashed_state,
                                       const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);
}

// slot 10
void generic_sync_anim_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(generic_sync_anim_state*, const mashed_state*))retail::sub_4E3820)(this, next_mashed_state);
}

// slot 11
e_state_exit_code generic_sync_anim_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(generic_sync_anim_state*, f32))retail::sub_4E3810)(this, delta_t);
}

// slot 12
void generic_sync_anim_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const generic_sync_anim_state*, info_node_desc_list*))retail::sub_4E3830)(this, &the_list);
}

// slot 24
bool generic_sync_anim_state::unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const {
    return ((bool (__thiscall*)(const generic_sync_anim_state*, als_inode*, als::category_id, als::layer_id))retail::sub_4FA410)(this, node, cat, layer);
}

// slot 25
als::category_id generic_sync_anim_state::get_master_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const generic_sync_anim_state*, als::category_id*))retail::sub_4E7320)(this, &category);

    return category;
}

// slot 26
als::category_id generic_sync_anim_state::get_slave_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const generic_sync_anim_state*, als::category_id*))retail::sub_4E73A0)(this, &category);

    return category;
}

// slot 27
als::layer_id generic_sync_anim_state::get_desired_als_layer() const {
    return ((als::layer_id (__thiscall*)(const generic_sync_anim_state*))retail::sub_4E7420)(this);
}

// slot 28
actor* generic_sync_anim_state::get_master() const {
    return ((actor* (__thiscall*)(const generic_sync_anim_state*))retail::sub_4F2BD0)(this);
}

// slot 29
actor* generic_sync_anim_state::get_slave() const {
    return ((actor* (__thiscall*)(const generic_sync_anim_state*))retail::sub_4F2B40)(this);
}

// slot 33
bool generic_sync_anim_state::unk_033() const {
    return ((bool (__thiscall*)(const generic_sync_anim_state*))retail::sub_4FA4B0)(this);
}

// ==== ai_obs_hop_sync_state ==== //

// slot 4
mash::virtual_types_key ai_obs_hop_sync_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const ai_obs_hop_sync_state*))retail::sub_4D0B50)(this);
}

// slot 6
bool ai_obs_hop_sync_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const ai_obs_hop_sync_state*, mash::virtual_types_key))retail::sub_4D0BF0)(this, parent_class);
}

// slot 10
void ai_obs_hop_sync_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(ai_obs_hop_sync_state*, const mashed_state*))retail::sub_4E7480)(this, next_mashed_state);
}

// ==== generic_slim_sync_anim_state ==== //

// slot 4
mash::virtual_types_key generic_slim_sync_anim_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const generic_slim_sync_anim_state*))retail::sub_4D0BE0)(this);
}

// slot 6
bool generic_slim_sync_anim_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const generic_slim_sync_anim_state*, mash::virtual_types_key))retail::sub_4D0BF0)(this, parent_class);
}

// sub_4F9FD0
void generic_slim_sync_anim_state::activate(      ai_state_machine* the_state_machine,
                                            const mashed_state*     the_mashed_state,
                                            const mashed_state*     prev_mashed_state,
                                            const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    auto* basic = (ai_basic_inode*)get_core()->get_info_node(info_node_type_ai_basic);

    if (basic->flags->get(ai_flag_do_capture)) {
        retail::sub_44F870((u32*)this, als::param_ai_rvb_capture_mode, 1.0f, true, true); // sync_anim_state::set_desired_param

        // whoever target 13 is stops being targetable
        if (basic->get_target_type(13) != simple_target_type_none) {
            if (ai_basic_inode* captured = get_ai_basic_inode_if_slim_actor(basic->get_target_entity(13)))
                captured->flags->set(ai_flag_ai_targetable, false);
        }
    }

    retail::sub_4E6B40((i32)get_actor(), (i32)this, 2);
}

// slot 10
void generic_slim_sync_anim_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(generic_slim_sync_anim_state*, const mashed_state*))retail::sub_4FA170)(this, next_mashed_state);
}

// slot 11
e_state_exit_code generic_slim_sync_anim_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(generic_slim_sync_anim_state*, f32))retail::sub_4E71E0)(this, delta_t);
}

// slot 12
void generic_slim_sync_anim_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const generic_slim_sync_anim_state*, info_node_desc_list*))retail::sub_4E72D0)(this, &the_list);
}

// sub_667F80
bool generic_slim_sync_anim_state::unk_016() const {
    return true;
}

// ==== plr_loco_sync_obstacle_avoid_state ==== //

// slot 1
void plr_loco_sync_obstacle_avoid_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*))retail::sub_54B240)(this);
}

// slot 2
void plr_loco_sync_obstacle_avoid_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*))retail::sub_54B270)(this);
}

// slot 3
void plr_loco_sync_obstacle_avoid_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_57C520)(this,
                                                                                                                                     mash_info,
                                                                                                                                     containing_class_ptr,
                                                                                                                                     buffer);
}

// slot 4
mash::virtual_types_key plr_loco_sync_obstacle_avoid_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*))retail::sub_5809E0)(this);
}

// slot 6
bool plr_loco_sync_obstacle_avoid_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4D38A0
i32 plr_loco_sync_obstacle_avoid_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_56CDC0
void plr_loco_sync_obstacle_avoid_state::activate(      ai_state_machine* the_state_machine,
                                                  const mashed_state*     the_mashed_state,
                                                  const mashed_state*     prev_mashed_state,
                                                  const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    unk_170 = state_exit_code_no_exit;

    early_exit_switch_time_callback.add(get_actor()->my_handle,
                                        als::references::early_exit_switch_time.read(),
                                        generic_event_callback::count_raise,
                                        nullptr);

    nav_exit_switch_time_callback.add(get_actor()->my_handle,
                                      als::references::nav_exit_switch_time.read(),
                                      generic_event_callback::count_raise,
                                      nullptr);
}

// slot 10
void plr_loco_sync_obstacle_avoid_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*, const mashed_state*))retail::sub_56D200)(this, next_mashed_state);
}

// slot 11
e_state_exit_code plr_loco_sync_obstacle_avoid_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_loco_sync_obstacle_avoid_state*, f32))retail::sub_57A600)(this, delta_t);
}

// slot 12
void plr_loco_sync_obstacle_avoid_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*, info_node_desc_list*))retail::sub_54B2D0)(this, &the_list);
}

// slot 24
bool plr_loco_sync_obstacle_avoid_state::unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const {
    return ((bool (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*, als_inode*, als::category_id, als::layer_id))retail::sub_48EEC0)(this, node, cat, layer);
}

// slot 25
als::category_id plr_loco_sync_obstacle_avoid_state::get_master_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*, als::category_id*))retail::sub_54B310)(this, &category);

    return category;
}

// slot 26
als::category_id plr_loco_sync_obstacle_avoid_state::get_slave_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*, als::category_id*))retail::sub_54B320)(this, &category);

    return category;
}

// slot 28
actor* plr_loco_sync_obstacle_avoid_state::get_master() const {
    return ((actor* (__thiscall*)(const plr_loco_sync_obstacle_avoid_state*))retail::sub_54B300)(this);
}

// sub_54B2F0
actor* plr_loco_sync_obstacle_avoid_state::get_slave() const {
    return get_actor();
}

// slot 32
void plr_loco_sync_obstacle_avoid_state::preslave_setup() {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*))retail::sub_56CE50)(this);
}

// slot 34
void plr_loco_sync_obstacle_avoid_state::unk_034(actor* the_actor) {
    ((void (__thiscall*)(plr_loco_sync_obstacle_avoid_state*, actor*))retail::sub_580A30)(this, the_actor);
}

// sub_689610
void plr_loco_sync_obstacle_avoid_state::unk_035(u32) {}
