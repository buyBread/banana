#include "retail.hh"
#include "game/wds/ai/states/car_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// ==== enter_car_state ==== //

// slot 1
void enter_car_state::construct_mashed_class() {
    ((void (__thiscall*)(enter_car_state*))retail::sub_48F5D0)(this);
}

// slot 2
void enter_car_state::destruct_mashed_class() {
    ((void (__thiscall*)(enter_car_state*))retail::sub_4E2280)(this);
}

// slot 3
void enter_car_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(enter_car_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                  mash_info,
                                                                                                                  containing_class_ptr,
                                                                                                                  buffer);
}

// slot 4
mash::virtual_types_key enter_car_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const enter_car_state*))retail::sub_4D2A30)(this);
}

// slot 6
bool enter_car_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const enter_car_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4BFE10
void enter_car_state::activate(      ai_state_machine* the_state_machine,
                               const mashed_state*     the_mashed_state,
                               const mashed_state*     prev_mashed_state,
                               const param_block*      extra_params) {

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    auto* car = (ai_car_inode*)get_core()->get_info_node(info_node_type_ai_car);

    // whoever already sits in the seat gets out
    if (auto* vehicle = (entity*)car->selected_car.resolve()) {
        if (vehicle->has_occupant_ifc()) {
            if (auto* occupant = (actor*)vehicle->occupant_ifc()->m_seat_occupants[car->selected_seat].resolve())
                ((ai_car_inode*)occupant->get_ai_core()->get_info_node(info_node_type_ai_car))->force_exit_car(false);
        }
    }
}

// slot 10
void enter_car_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(enter_car_state*, const mashed_state*))retail::sub_4BFEF0)(this, next_mashed_state);
}

// slot 11
e_state_exit_code enter_car_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(enter_car_state*, f32))retail::sub_4BE330)(this, delta_t);
}

// slot 12
void enter_car_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const enter_car_state*, info_node_desc_list*))retail::sub_48EFD0)(this, &the_list);
}

// slot 24
bool enter_car_state::unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const {
    return ((bool (__thiscall*)(const enter_car_state*, als_inode*, als::category_id, als::layer_id))retail::sub_48EEC0)(this, node, cat, layer);
}

// slot 25
als::category_id enter_car_state::get_master_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const enter_car_state*, als::category_id*))retail::sub_4937A0)(this, &category);

    return category;
}

// slot 26
als::category_id enter_car_state::get_slave_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const enter_car_state*, als::category_id*))retail::sub_4D2A10)(this, &category);

    return category;
}

// slot 27
als::layer_id enter_car_state::get_desired_als_layer() const {
    return ((als::layer_id (__thiscall*)(const enter_car_state*))retail::sub_493880)(this);
}

// slot 28
actor* enter_car_state::get_master() const {
    return ((actor* (__thiscall*)(const enter_car_state*))retail::sub_493760)(this);
}

// sub_4D2A40
actor* enter_car_state::get_slave() const {
    return get_actor();
}

// ==== exit_car_state ==== //

// slot 1
void exit_car_state::construct_mashed_class() {
    ((void (__thiscall*)(exit_car_state*))retail::sub_48F5D0)(this);
}

// slot 2
void exit_car_state::destruct_mashed_class() {
    ((void (__thiscall*)(exit_car_state*))retail::sub_4E2280)(this);
}

// slot 3
void exit_car_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(exit_car_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_48EA30)(this,
                                                                                                                 mash_info,
                                                                                                                 containing_class_ptr,
                                                                                                                 buffer);
}

// slot 4
mash::virtual_types_key exit_car_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const exit_car_state*))retail::sub_4D2AD0)(this);
}

// slot 6
bool exit_car_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const exit_car_state*, mash::virtual_types_key))retail::sub_5809F0)(this, parent_class);
}

// sub_4D4950
i32 exit_car_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_4BE3E0
void exit_car_state::activate(      ai_state_machine* the_state_machine,
                              const mashed_state*     the_mashed_state,
                              const mashed_state*     prev_mashed_state,
                              const param_block*      extra_params) {

    auto* car = (ai_car_inode*)the_state_machine->get_core()->get_info_node(info_node_type_ai_car);

    selected_car  = car->selected_car.resolve() ? car->selected_car.resolve()->my_handle : arch_base_vhandle();
    selected_seat = car->selected_seat;

    retail::sub_653EB0((u32*)the_state_machine->get_core()->get_actor(), 0); // attaches it to nothing, keeping its world po

    sync_anim_state::activate(the_state_machine, the_mashed_state, prev_mashed_state, extra_params);

    retail::sub_4BDFD0((i32)car, 0, nullptr);
}

// slot 10
void exit_car_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(exit_car_state*, const mashed_state*))retail::sub_4BFF50)(this, next_mashed_state);
}

// slot 12
void exit_car_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const exit_car_state*, info_node_desc_list*))retail::sub_48EFD0)(this, &the_list);
}

// slot 24
bool exit_car_state::unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const {
    return ((bool (__thiscall*)(const exit_car_state*, als_inode*, als::category_id, als::layer_id))retail::sub_48EEC0)(this, node, cat, layer);
}

// slot 25
als::category_id exit_car_state::get_master_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const exit_car_state*, als::category_id*))retail::sub_48EEF0)(this, &category);

    return category;
}

// slot 26
als::category_id exit_car_state::get_slave_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const exit_car_state*, als::category_id*))retail::sub_4D2A10)(this, &category);

    return category;
}

// slot 27
als::layer_id exit_car_state::get_desired_als_layer() const {
    return ((als::layer_id (__thiscall*)(const exit_car_state*))retail::sub_48EF90)(this);
}

// slot 28
actor* exit_car_state::get_master() const {
    return ((actor* (__thiscall*)(const exit_car_state*))retail::sub_54B300)(this);
}

// sub_4D2A40
actor* exit_car_state::get_slave() const {
    return get_actor();
}
