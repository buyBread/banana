#include "retail.hh"
#include "game/wds/ai/states/cpu_combat_states.hh"

using namespace treyarch;

// ==== cpu_combat_state ==== //

// slot 1
void cpu_combat_state::construct_mashed_class() {
    ((void (__thiscall*)(cpu_combat_state*))retail::sub_5488E0)(this);
}

// slot 2
void cpu_combat_state::destruct_mashed_class() {
    ((void (__thiscall*)(cpu_combat_state*))retail::sub_54BB40)(this);
}

// slot 3
void cpu_combat_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(cpu_combat_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_4E2270)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key cpu_combat_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const cpu_combat_state*))retail::sub_4D08B0)(this);
}

// slot 6
bool cpu_combat_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const cpu_combat_state*, mash::virtual_types_key))retail::sub_4D4120)(this, parent_class);
}

// sub_4D08C0
i32 cpu_combat_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void cpu_combat_state::activate(      ai_state_machine* the_state_machine,
                                const mashed_state*     the_mashed_state,
                                const mashed_state*     prev_mashed_state,
                                const param_block*      extra_params) {

    ((void (__thiscall*)(cpu_combat_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_4F87C0)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void cpu_combat_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(cpu_combat_state*, const mashed_state*))retail::sub_4F88A0)(this, next_mashed_state);
}

// slot 11
e_state_exit_code cpu_combat_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(cpu_combat_state*, f32))retail::sub_4F90C0)(this, delta_t);
}

// slot 12
void cpu_combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const cpu_combat_state*, info_node_desc_list*))retail::sub_4E36A0)(this, &the_list);
}

// slot 20
void cpu_combat_state::update_next_move() {
    ((void (__thiscall*)(cpu_combat_state*))retail::sub_4F7470)(this);
}

// slot 21
e_combat_move cpu_combat_state::get_next_move(f32 delta_t, i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(cpu_combat_state*, f32, i32*))retail::sub_4EEC00)(this, delta_t, next_script_move_num);
}

// slot 22
e_combat_move cpu_combat_state::get_initial_move(i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(cpu_combat_state*, i32*))retail::sub_4EEBD0)(this, next_script_move_num);
}

// slot 24
void cpu_combat_state::add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) {
    ((void (__thiscall*)(cpu_combat_state*, als_inode*, const combo_move_data*))retail::sub_4EF8E0)(this, my_als, combo_move);
}

// ==== civ_beat_up_cpu_combat_state ==== //

// slot 1
void civ_beat_up_cpu_combat_state::construct_mashed_class() {
    ((void (__thiscall*)(civ_beat_up_cpu_combat_state*))retail::sub_48F630)(this);
}

// slot 2
void civ_beat_up_cpu_combat_state::destruct_mashed_class() {
    ((void (__thiscall*)(civ_beat_up_cpu_combat_state*))retail::sub_48F640)(this);
}

// slot 3
void civ_beat_up_cpu_combat_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(civ_beat_up_cpu_combat_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48F650)(this,
                                                                                                                               mash_info,
                                                                                                                               containing_class_ptr,
                                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key civ_beat_up_cpu_combat_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const civ_beat_up_cpu_combat_state*))retail::sub_4D5E10)(this);
}

// slot 6
bool civ_beat_up_cpu_combat_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const civ_beat_up_cpu_combat_state*, mash::virtual_types_key))retail::sub_4D5E20)(this, parent_class);
}

// slot 10
void civ_beat_up_cpu_combat_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(civ_beat_up_cpu_combat_state*, const mashed_state*))retail::sub_497EC0)(this, next_mashed_state);
}

// slot 12
void civ_beat_up_cpu_combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const civ_beat_up_cpu_combat_state*, info_node_desc_list*))retail::sub_48F740)(this, &the_list);
}

// ==== generic_slim_combat_state ==== //

// slot 1
void generic_slim_combat_state::construct_mashed_class() {
    ((void (__thiscall*)(generic_slim_combat_state*))retail::sub_5488E0)(this);
}

// slot 2
void generic_slim_combat_state::destruct_mashed_class() {
    ((void (__thiscall*)(generic_slim_combat_state*))retail::sub_54BB40)(this);
}

// slot 3
void generic_slim_combat_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(generic_slim_combat_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_4E2270)(this,
                                                                                                                            mash_info,
                                                                                                                            containing_class_ptr,
                                                                                                                            buffer);
}

// slot 4
mash::virtual_types_key generic_slim_combat_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const generic_slim_combat_state*))retail::sub_4D0A60)(this);
}

// slot 6
bool generic_slim_combat_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const generic_slim_combat_state*, mash::virtual_types_key))retail::sub_4D4120)(this, parent_class);
}

// slot 9
void generic_slim_combat_state::activate(      ai_state_machine* the_state_machine,
                                         const mashed_state*     the_mashed_state,
                                         const mashed_state*     prev_mashed_state,
                                         const param_block*      extra_params) {

    ((void (__thiscall*)(generic_slim_combat_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_4E6FC0)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void generic_slim_combat_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(generic_slim_combat_state*, const mashed_state*))retail::sub_4E3780)(this, next_mashed_state);
}

// slot 11
e_state_exit_code generic_slim_combat_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(generic_slim_combat_state*, f32))retail::sub_4F9EA0)(this, delta_t);
}

// slot 12
void generic_slim_combat_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const generic_slim_combat_state*, info_node_desc_list*))retail::sub_4E37B0)(this, &the_list);
}

// sub_667F80
bool generic_slim_combat_state::unk_016() const {
    return true;
}

// slot 20
void generic_slim_combat_state::update_next_move() {
    ((void (__thiscall*)(generic_slim_combat_state*))retail::sub_4F2A60)(this);
}

// slot 21
e_combat_move generic_slim_combat_state::get_next_move(f32 delta_t, i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(generic_slim_combat_state*, f32, i32*))retail::sub_4E70E0)(this, delta_t, next_script_move_num);
}

// slot 22
e_combat_move generic_slim_combat_state::get_initial_move(i32* next_script_move_num) {
    return ((e_combat_move (__thiscall*)(generic_slim_combat_state*, i32*))retail::sub_4E37F0)(this, next_script_move_num);
}

// slot 46
string_hash generic_slim_combat_state::get_combat_als_category() const {
    string_hash category;

    ((string_hash* (__thiscall*)(const generic_slim_combat_state*, string_hash*))retail::sub_4E7160)(this, &category);

    return category;
}
