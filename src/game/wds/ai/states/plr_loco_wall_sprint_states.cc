#include "retail.hh"
#include "game/wds/ai/states/plr_loco_wall_sprint_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/spline.hh"

using namespace treyarch;

// ==== plr_loco_wall_sprint_base_state ==== //

// slot 1
void plr_loco_wall_sprint_base_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*))retail::sub_54B710)(this);
}

// slot 2
void plr_loco_wall_sprint_base_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*))retail::sub_450E50)(this);
}

// slot 3
void plr_loco_wall_sprint_base_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_54B5E0)(this,
                                                                                                                                  mash_info,
                                                                                                                                  containing_class_ptr,
                                                                                                                                  buffer);
}

// slot 4
mash::virtual_types_key plr_loco_wall_sprint_base_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_loco_wall_sprint_base_state*))retail::sub_4D2410)(this);
}

// slot 6
bool plr_loco_wall_sprint_base_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_loco_wall_sprint_base_state*, mash::virtual_types_key))retail::sub_5808A0)(this, parent_class);
}

// sub_4BF700
i32 plr_loco_wall_sprint_base_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_loco_wall_sprint_base_state::activate(      ai_state_machine* the_state_machine,
                                               const mashed_state*     the_mashed_state,
                                               const mashed_state*     prev_mashed_state,
                                               const param_block*      extra_params) {

    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_54A760)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_loco_wall_sprint_base_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*, const mashed_state*))retail::sub_48F000)(this, next_mashed_state);
}

// slot 11
e_state_exit_code plr_loco_wall_sprint_base_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_loco_wall_sprint_base_state*, f32))retail::sub_577AC0)(this, delta_t);
}

// slot 12
void plr_loco_wall_sprint_base_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const plr_loco_wall_sprint_base_state*, info_node_desc_list*))retail::sub_54A800)(this, &the_list);
}

// slot 20
void plr_loco_wall_sprint_base_state::map_controls(i32 config) {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*, i32))retail::sub_54A7A0)(this, config);
}

// slot 21
void plr_loco_wall_sprint_base_state::set_player_mode(actor* act) {
    ((void (__thiscall*)(plr_loco_wall_sprint_base_state*, actor*))retail::sub_4D2420)(this, act);
}

// slot 22
vector3d plr_loco_wall_sprint_base_state::get_z_facing() const {
    vector3d facing;

    ((vector3d* (__thiscall*)(const plr_loco_wall_sprint_base_state*, vector3d*))retail::sub_4D2450)(this, &facing);

    return facing;
}

// slot 23
vector3d plr_loco_wall_sprint_base_state::get_y_facing() const {
    vector3d facing;

    ((vector3d* (__thiscall*)(const plr_loco_wall_sprint_base_state*, vector3d*))retail::sub_4D2480)(this, &facing);

    return facing;
}

// slot 24
vector3d plr_loco_wall_sprint_base_state::get_x_facing() const {
    vector3d facing;

    ((vector3d* (__thiscall*)(const plr_loco_wall_sprint_base_state*, vector3d*))retail::sub_4D24B0)(this, &facing);

    return facing;
}

// ==== plr_loco_wall_sprint_transition_state ==== //

// slot 1
void plr_loco_wall_sprint_transition_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*))retail::sub_54A720)(this);
}

// slot 2
void plr_loco_wall_sprint_transition_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*))retail::sub_54A740)(this);
}

// slot 3
void plr_loco_wall_sprint_transition_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_57B7E0)(this,
                                                                                                                                        mash_info,
                                                                                                                                        containing_class_ptr,
                                                                                                                                        buffer);
}

// slot 4
mash::virtual_types_key plr_loco_wall_sprint_transition_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_loco_wall_sprint_transition_state*))retail::sub_4D26F0)(this);
}

// slot 6
bool plr_loco_wall_sprint_transition_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_loco_wall_sprint_transition_state*, mash::virtual_types_key))retail::sub_4D2700)(this, parent_class);
}

// sub_4D2740
i32 plr_loco_wall_sprint_transition_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_loco_wall_sprint_transition_state::activate(      ai_state_machine* the_state_machine,
                                                     const mashed_state*     the_mashed_state,
                                                     const mashed_state*     prev_mashed_state,
                                                     const param_block*      extra_params) {

    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_580180)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// sub_580EC0
void plr_loco_wall_sprint_transition_state::deactivate(const mashed_state* next_mashed_state) {
    enhanced_state::deactivate(next_mashed_state);

    auto* sprint = (wall_sprint_inode*)get_core()->get_info_node(info_node_type_wall_sprint);

    sprint->unk_060 = sprint->unk_05c;
    sprint->unk_05c = 0;

    if (get_actor()->has_ifc(entity_ifc_standing)) {
        get_actor()->standing_ifc()->flags &= ~standing_interface_flag_adv_standing;
        get_actor()->standing_ifc()->flags &= ~standing_interface_flag_wall_sprinting;
    }

    if (get_actor()->has_ifc(entity_ifc_physical)) {
        if (sprint->unk_060 == 10)
            get_actor()->physical_ifc()->set_velocity(unk_054);

        get_actor()->physical_ifc()->enable(true);
        get_actor()->physical_ifc()->set_gravity(true);
    }

    player_interface* player = get_actor()->player_ifc();

    player->unk_114(0);
    player->unk_116(0);
    player->unk_119(0);
    player->unk_112(false, false);

    if (path) {
        delete path;

        path = nullptr;
    }

    web_shoot_callback  .remove(get_actor()->my_handle, references::web_shoot.read());
    web_release_callback.remove(get_actor()->my_handle, references::web_release.read());

    unk_027();
}

// slot 11
e_state_exit_code plr_loco_wall_sprint_transition_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_loco_wall_sprint_transition_state*, f32))retail::sub_5797B0)(this, delta_t);
}

// slot 25
void plr_loco_wall_sprint_transition_state::unk_025() {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*))retail::sub_5810C0)(this);
}

// slot 26
void plr_loco_wall_sprint_transition_state::unk_026() {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*))retail::sub_565370)(this);
}

// slot 27
void plr_loco_wall_sprint_transition_state::unk_027() {
    ((void (__thiscall*)(plr_loco_wall_sprint_transition_state*))retail::sub_565530)(this);
}
