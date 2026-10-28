#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// sub_4C7740
ai_basic_inode* treyarch::get_ai_basic_inode(arch_base* the_entity) { // asks the actor for its core twice
    if (!the_entity || !the_entity->is_an_actor() || !((actor*)the_entity)->get_ai_core())
        return nullptr;

    return (ai_basic_inode*)((actor*)the_entity)->get_ai_core()->get_info_node(info_node_type_ai_basic);
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
