#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_4E2C10
f32 ai_basic_inode::get_actual_car_seat() const {
    return my_param_block.get_optional_pb_float(actual_car_seat_key, -1.0f, nullptr);
}

// sub_4E2E50
f32 ai_basic_inode::get_spread_factor() const {
    return my_param_block.get_optional_pb_float(spread_factor_key, 1.0f, nullptr);
}

// sub_4E2E80
vector3d ai_basic_inode::get_target_offset_vector() const {
    return my_param_block.get_optional_pb_vector3d(target_offset_vector_key, references::unk_00fc5f00.get(), nullptr);
}

// sub_4E2D30
f32 ai_basic_inode::get_base_damage_scalar() const {
    return my_param_block.get_optional_pb_float(base_damage_scalar_key, 1.0f, nullptr);
}

// sub_4C7740
ai_basic_inode* treyarch::get_ai_basic_inode(arch_base* the_entity) { // asks the actor for its core twice
    if (!the_entity || !the_entity->is_an_actor() || !((actor*)the_entity)->get_ai_core())
        return nullptr;

    return (ai_basic_inode*)((actor*)the_entity)->get_ai_core()->get_info_node(info_node_type_ai_basic);
}

// sub_4C77B0
ai_basic_inode* treyarch::get_ai_basic_inode_if_slim_actor(arch_base* the_entity) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (inode && inode->get_flag(ai_flag_is_slim_actor))
        return inode;

    return nullptr;
}

// sub_4C74D0
void treyarch::rvb_set_needs_repathfind(arch_base* the_entity) { // the entity is taken for an actor with a core
    ai_core* core = ((actor*)the_entity)->get_ai_core();

    for (e_info_node_type type : { info_node_type_swift_surface_layer, info_node_type_swift_flight_layer }) {
        auto swift_layer = (loco_inode*)core->get_info_node(type);

        if (swift_layer && swift_layer->is_using_swift())
            swift_layer->set_needs_repathfind(true, true);
    }
}

// sub_4CB170
void treyarch::rvb_reset_behavior(arch_base* the_entity) {
    if (ai_basic_inode* inode = get_ai_basic_inode(the_entity))
        inode->reset();
}

// sub_4CB790
void treyarch::rvb_set_var(arch_base* the_entity, const string_hash* name, f32 value) {
    if (ai_basic_inode* inode = get_ai_basic_inode(the_entity))
        inode->set_var_float(name, value);
}

// sub_4CB7C0
f32 treyarch::rvb_get_var(arch_base* the_entity, const string_hash* name) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    return inode ? inode->get_var_float(name, nullptr) : 0.0f;
}

// sub_4DFAE0
void treyarch::rvb_set_flag(arch_base* the_entity, const string_hash* name, bool value) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (!inode)
        return;

    inode->set_flag(find_ai_flag(name->source_hash_code), value);
}

// sub_4DFB20
bool treyarch::rvb_get_flag(arch_base* the_entity, const string_hash* name) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (!inode)
        return false;

    return inode->get_flag(find_ai_flag(name->source_hash_code));
}
