#include "retail.hh"
#include "game/wds/ai/states/carry_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// ==== pick_up_ai_state ==== //

// slot 1
void pick_up_ai_state::construct_mashed_class() {
    ((void (__thiscall*)(pick_up_ai_state*))retail::sub_4B5C60)(this);
}

// slot 2
void pick_up_ai_state::destruct_mashed_class() {
    ((void (__thiscall*)(pick_up_ai_state*))retail::sub_4E2280)(this);
}

// slot 3
void pick_up_ai_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(pick_up_ai_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key pick_up_ai_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const pick_up_ai_state*))retail::sub_4C0B80)(this);
}

// slot 6
bool pick_up_ai_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const pick_up_ai_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4C0B90
i32 pick_up_ai_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_492650
void pick_up_ai_state::activate(      ai_state_machine* the_state_machine,
                                const mashed_state*     the_mashed_state,
                                const mashed_state*     prev_mashed_state,
                                const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);
}

// sub_4BF1F0
void pick_up_ai_state::deactivate(const mashed_state* next_mashed_state) {
    auto* carry = (std_carry_inode*)get_core()->get_info_node(info_node_type_std_carry);

    if (!unk_16c.null()) {
        if (auto* carried = (actor*)unk_16c.resolve()) {
            if (carried->get_ai_core()) {
                if (auto* carried_carry = (std_carry_inode*)carried->get_ai_core()->get_info_node(info_node_type_std_carry))
                    carry->add_slave(carried_carry, false);
            }
        }
    }

    if (engaged_slave_mode)
        engaged_slave_mode = false;

    sync_anim_state::deactivate(next_mashed_state);
}

// slot 11
e_state_exit_code pick_up_ai_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(pick_up_ai_state*, f32))retail::sub_4A2370)(this, delta_t);
}

// slot 12
void pick_up_ai_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const pick_up_ai_state*, info_node_desc_list*))retail::sub_492660)(this, &the_list);
}

// slot 29
actor* pick_up_ai_state::get_slave() const {
    return ((actor* (__thiscall*)(const pick_up_ai_state*))retail::sub_4A2400)(this);
}

// slot 32
void pick_up_ai_state::preslave_setup() {
    ((void (__thiscall*)(pick_up_ai_state*))retail::sub_4A21C0)(this);
}

// ==== plr_pick_up_ai_state ==== //

// slot 1
void plr_pick_up_ai_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_pick_up_ai_state*))retail::sub_54B6B0)(this);
}

// slot 2
void plr_pick_up_ai_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_pick_up_ai_state*))retail::sub_54B690)(this);
}

// slot 3
void plr_pick_up_ai_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_pick_up_ai_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_54B6A0)(this,
                                                                                                                       mash_info,
                                                                                                                       containing_class_ptr,
                                                                                                                       buffer);
}

// slot 4
mash::virtual_types_key plr_pick_up_ai_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_pick_up_ai_state*))retail::sub_4D48F0)(this);
}

// slot 6
bool plr_pick_up_ai_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_pick_up_ai_state*, mash::virtual_types_key))retail::sub_4D4900)(this, parent_class);
}

// sub_4D4950
i32 plr_pick_up_ai_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_54B9E0
void plr_pick_up_ai_state::activate(      ai_state_machine* the_state_machine,
                                    const mashed_state*     the_mashed_state,
                                    const mashed_state*     prev_mashed_state,
                                    const param_block*      extra_params) {

    pick_up_ai_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    unk_034(get_actor());

    can_dodge = can_handle_exit_code(state_exit_code_dodge);
}

// slot 11
e_state_exit_code plr_pick_up_ai_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_pick_up_ai_state*, f32))retail::sub_5726A0)(this, delta_t);
}

// slot 34
void plr_pick_up_ai_state::unk_034(actor* the_actor) {
    ((void (__thiscall*)(plr_pick_up_ai_state*, actor*))retail::sub_4D4960)(this, the_actor);
}

// ==== put_down_ai_state ==== //

// slot 1
void put_down_ai_state::construct_mashed_class() {
    ((void (__thiscall*)(put_down_ai_state*))retail::sub_4B5C60)(this);
}

// slot 2
void put_down_ai_state::destruct_mashed_class() {
    ((void (__thiscall*)(put_down_ai_state*))retail::sub_4E2280)(this);
}

// slot 3
void put_down_ai_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(put_down_ai_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                    mash_info,
                                                                                                                    containing_class_ptr,
                                                                                                                    buffer);
}

// slot 4
mash::virtual_types_key put_down_ai_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const put_down_ai_state*))retail::sub_4C0C70)(this);
}

// slot 6
bool put_down_ai_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const put_down_ai_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4D4950
i32 put_down_ai_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_4A2420
void put_down_ai_state::activate(      ai_state_machine* the_state_machine,
                                 const mashed_state*     the_mashed_state,
                                 const mashed_state*     prev_mashed_state,
                                 const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    engaged_slave_mode = true;

    if (auto* carried = (actor*)unk_16c.resolve()) {
        if (auto* carried_als = (als_inode*)carried->get_ai_core()->get_info_node(info_node_type_als))
            retail::sub_44F370((u32**)carried_als, 9);
    }

    auto* carry = (std_carry_inode*)get_core()->get_info_node(info_node_type_std_carry);

    if (!carry->carry_slaves->size()) {
        if (auto* own_als = (als_inode*)get_core()->get_info_node(info_node_type_als))
            retail::sub_44F370((u32**)own_als, 9);
    }

    unk_170 = !get_actor()->has_ifc(entity_ifc_standing) || !get_actor()->standing_ifc()->is_effectively_standing();
}

// slot 10
void put_down_ai_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(put_down_ai_state*, const mashed_state*))retail::sub_4A2560)(this, next_mashed_state);
}

// slot 11
e_state_exit_code put_down_ai_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(put_down_ai_state*, f32))retail::sub_4A2890)(this, delta_t);
}

// slot 12
void put_down_ai_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const put_down_ai_state*, info_node_desc_list*))retail::sub_492660)(this, &the_list);
}

// slot 29
actor* put_down_ai_state::get_slave() const {
    return ((actor* (__thiscall*)(const put_down_ai_state*))retail::sub_4A2400)(this);
}

// sub_71F870
bool put_down_ai_state::engage_ai_slave_mode() const {
    return false;
}

// slot 32
void put_down_ai_state::preslave_setup() {
    ((void (__thiscall*)(put_down_ai_state*))retail::sub_4BA750)(this);
}

// ==== plr_put_down_ai_state ==== //

// slot 1
void plr_put_down_ai_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_put_down_ai_state*))retail::sub_54B6B0)(this);
}

// slot 2
void plr_put_down_ai_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_put_down_ai_state*))retail::sub_54B690)(this);
}

// slot 3
void plr_put_down_ai_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_put_down_ai_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_54B6A0)(this,
                                                                                                                        mash_info,
                                                                                                                        containing_class_ptr,
                                                                                                                        buffer);
}

// slot 4
mash::virtual_types_key plr_put_down_ai_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_put_down_ai_state*))retail::sub_4D49E0)(this);
}

// slot 6
bool plr_put_down_ai_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_put_down_ai_state*, mash::virtual_types_key))retail::sub_4D49F0)(this, parent_class);
}

// sub_487020
i32 plr_put_down_ai_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_54BA30
void plr_put_down_ai_state::activate(      ai_state_machine* the_state_machine,
                                     const mashed_state*     the_mashed_state,
                                     const mashed_state*     prev_mashed_state,
                                     const param_block*      extra_params) {

    put_down_ai_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    unk_034(get_actor());

    can_dodge = can_handle_exit_code(state_exit_code_dodge);
}

// slot 10
void plr_put_down_ai_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_put_down_ai_state*, const mashed_state*))retail::sub_572760)(this, next_mashed_state);
}

// slot 11
e_state_exit_code plr_put_down_ai_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_put_down_ai_state*, f32))retail::sub_572700)(this, delta_t);
}

// slot 34
void plr_put_down_ai_state::unk_034(actor* the_actor) {
    ((void (__thiscall*)(plr_put_down_ai_state*, actor*))retail::sub_4D4BA0)(this, the_actor);
}
