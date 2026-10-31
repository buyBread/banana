#include "retail.hh"
#include "game/wds/ai/states/civilian_agressor_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/variance_variable.hh"

using namespace treyarch;

// ==== hold_hostage_state ==== //

// slot 1
void hold_hostage_state::construct_mashed_class() {
    ((void (__thiscall*)(hold_hostage_state*))retail::sub_48F5D0)(this);
}

// slot 2
void hold_hostage_state::destruct_mashed_class() {
    ((void (__thiscall*)(hold_hostage_state*))retail::sub_4E2280)(this);
}

// slot 3
void hold_hostage_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(hold_hostage_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                     mash_info,
                                                                                                                     containing_class_ptr,
                                                                                                                     buffer);
}

// slot 4
mash::virtual_types_key hold_hostage_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const hold_hostage_state*))retail::sub_4D6620)(this);
}

// slot 6
bool hold_hostage_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const hold_hostage_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_487020
i32 hold_hostage_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_495E60
void hold_hostage_state::activate(      ai_state_machine* the_state_machine,
                                  const mashed_state*     the_mashed_state,
                                  const mashed_state*     prev_mashed_state,
                                  const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    u32 &guard = references::hold_hostage_line_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        references::hold_hostage_line.get().initialize(mash::ALLOCATED, "hold_hostage");
        retail::sub_ADE5FD(retail::sub_B69D20); // the game's CRT atexit
    }

    retail::sub_4C6490((u32*)get_core(), (i32*)&references::hold_hostage_line.get()); // hands it to the voice box

    unk_16c = false;
    unk_16d = false;
    unk_16e = false;
    unk_170 = -1.0f;
    unk_174 = false;

    auto* agressor = (ai_civilian_agressor_inode*)get_core()->get_info_node(info_node_type_ai_civilian_agressor);

    agressor->lock.acquire();
    agressor->unk_07c = false;

    if (agressor->unk_026()) {
        if (retail::sub_494B00((u32*)agressor) || retail::sub_494930((u32*)agressor) || retail::sub_494760((u32*)agressor))
            retail::sub_494CC0((u32*)agressor);

        // the victim's als follows ours
        auto* own_als    = (als_inode*)get_core()->get_info_node(info_node_type_als);
        auto* victim_als = (als_inode*)((info_node*)retail::sub_4941F0(agressor))->get_core()->get_info_node(info_node_type_als);

        if (victim_als && own_als)
            ((als::animation_logic_system_interface*)victim_als->my_als)->set_coordinator((als::animation_logic_system_interface*)own_als->my_als, 0);

        if (auto* hostage = (actor*)agressor->unk_026()) {
            if (hostage->my_ifc_storage->get_ifc(entity_ifc_damage))
                ((damage_interface*)hostage->my_ifc_storage->get_ifc(entity_ifc_damage))->flags |= damage_ifc_flag_unk_40000000;

            if (hostage->get_ai_core())
                hostage->get_ai_core()->flags |= ai_core_flag_unk_00020000;
        }
    }

    get_actor()->damage_ifc()->flags |= damage_ifc_flag_unk_40000000;

    agressor->lock.release();
}

// slot 10
void hold_hostage_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(hold_hostage_state*, const mashed_state*))retail::sub_4960C0)(this, next_mashed_state);
}

// slot 11
e_state_exit_code hold_hostage_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(hold_hostage_state*, f32))retail::sub_496330)(this, delta_t);
}

// slot 12
void hold_hostage_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const hold_hostage_state*, info_node_desc_list*))retail::sub_48F570)(this, &the_list);
}

// slot 29
actor* hold_hostage_state::get_slave() const {
    return ((actor* (__thiscall*)(const hold_hostage_state*))retail::sub_496A00)(this);
}

// ==== harass_victim_state ==== //

// slot 1
void harass_victim_state::construct_mashed_class() {
    ((void (__thiscall*)(harass_victim_state*))retail::sub_48F5D0)(this);
}

// slot 2
void harass_victim_state::destruct_mashed_class() {
    ((void (__thiscall*)(harass_victim_state*))retail::sub_4E2280)(this);
}

// slot 3
void harass_victim_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(harass_victim_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                      mash_info,
                                                                                                                      containing_class_ptr,
                                                                                                                      buffer);
}

// slot 4
mash::virtual_types_key harass_victim_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const harass_victim_state*))retail::sub_4D62E0)(this);
}

// slot 6
bool harass_victim_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const harass_victim_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_496EF0
void harass_victim_state::activate(      ai_state_machine* the_state_machine,
                                   const mashed_state*     the_mashed_state,
                                   const mashed_state*     prev_mashed_state,
                                   const param_block*      extra_params) {

    auto* agressor = (ai_civilian_agressor_inode*)the_state_machine->get_core()->get_info_node(info_node_type_ai_civilian_agressor);

    agressor->lock.acquire();

    if (agressor->unk_026()) {
        if (retail::sub_4943C0((u32*)agressor) || retail::sub_494930((u32*)agressor) || retail::sub_494760((u32*)agressor))
            retail::sub_4944B0((u32*)agressor);

        // the victim's als follows ours
        auto* own_als    = (als_inode*)the_state_machine->get_core()->get_info_node(info_node_type_als);
        auto* victim_als = (als_inode*)((info_node*)retail::sub_4941F0(agressor))->get_core()->get_info_node(info_node_type_als);

        if (victim_als && own_als)
            ((als::animation_logic_system_interface*)victim_als->my_als)->set_coordinator((als::animation_logic_system_interface*)own_als->my_als, 0);
    }

    agressor->lock.release();

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    u32 &guard = references::harass_victim_line_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        references::harass_victim_line.get().initialize(mash::ALLOCATED, "harass_victim");
        retail::sub_ADE5FD(retail::sub_B69DB0); // the game's CRT atexit
    }

    retail::sub_4C6490((u32*)get_core(), (i32*)&references::harass_victim_line.get()); // hands it to the voice box
}

// slot 10
void harass_victim_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(harass_victim_state*, const mashed_state*))retail::sub_4970B0)(this, next_mashed_state);
}

// slot 11
e_state_exit_code harass_victim_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(harass_victim_state*, f32))retail::sub_4972A0)(this, delta_t);
}

// slot 12
void harass_victim_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const harass_victim_state*, info_node_desc_list*))retail::sub_48F5E0)(this, &the_list);
}

// slot 29
actor* harass_victim_state::get_slave() const {
    return ((actor* (__thiscall*)(const harass_victim_state*))retail::sub_496A00)(this);
}

// ==== hold_abductee_state ==== //

// slot 1
void hold_abductee_state::construct_mashed_class() {
    ((void (__thiscall*)(hold_abductee_state*))retail::sub_4905E0)(this);
}

// slot 2
void hold_abductee_state::destruct_mashed_class() {
    ((void (__thiscall*)(hold_abductee_state*))retail::sub_490600)(this);
}

// slot 3
void hold_abductee_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(hold_abductee_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_4B6870)(this,
                                                                                                                      mash_info,
                                                                                                                      containing_class_ptr,
                                                                                                                      buffer);
}

// slot 4
mash::virtual_types_key hold_abductee_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const hold_abductee_state*))retail::sub_4D5A00)(this);
}

// slot 6
bool hold_abductee_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const hold_abductee_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4D6AF0
i32 hold_abductee_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_49C200
void hold_abductee_state::activate(      ai_state_machine* the_state_machine,
                                   const mashed_state*     the_mashed_state,
                                   const mashed_state*     prev_mashed_state,
                                   const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    u32 &guard = references::abduct_line_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        references::abduct_line.get().initialize(mash::ALLOCATED, "abduct");
        retail::sub_ADE5FD(retail::sub_B6A040); // the game's CRT atexit
    }

    retail::sub_4C6490((u32*)get_core(), (i32*)&references::abduct_line.get()); // hands it to the voice box

    get_core()->my_loco_inode->set_allow_als_category_request(false);

    auto* agressor = (ai_civilian_agressor_inode*)get_core()->get_info_node(info_node_type_ai_civilian_agressor);

    agressor->lock.acquire();

    if (agressor->unk_026()) {
        if (retail::sub_494760((u32*)agressor) || retail::sub_494DA0((u32*)agressor) || retail::sub_494B00((u32*)agressor))
            retail::sub_494850((u32*)agressor);

        // the victim's als follows ours
        auto* own_als    = (als_inode*)get_core()->get_info_node(info_node_type_als);
        auto* victim_als = (als_inode*)((info_node*)retail::sub_4941F0(agressor))->get_core()->get_info_node(info_node_type_als);

        if (victim_als && own_als)
            ((als::animation_logic_system_interface*)victim_als->my_als)->set_coordinator((als::animation_logic_system_interface*)own_als->my_als, 0);
    }

    unk_16c = 0;
    unk_170 = 0;
    unk_174 = variance_variable<f32>{ 1.5f, 0.75f }.get_random_value_inline();
    unk_178 = false;

    agressor->lock.release();
}

// slot 10
void hold_abductee_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(hold_abductee_state*, const mashed_state*))retail::sub_49C430)(this, next_mashed_state);
}

// slot 11
e_state_exit_code hold_abductee_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(hold_abductee_state*, f32))retail::sub_49C600)(this, delta_t);
}

// slot 12
void hold_abductee_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const hold_abductee_state*, info_node_desc_list*))retail::sub_48F5E0)(this, &the_list);
}

// slot 29
actor* hold_abductee_state::get_slave() const {
    return ((actor* (__thiscall*)(const hold_abductee_state*))retail::sub_496A00)(this);
}
