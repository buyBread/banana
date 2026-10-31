#include <new>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_vehicle.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/wds/references.hh"
#include "game/math/references.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_4EF830
// inlined @ sub_4F9FD0
e_simple_target_type ai_basic_inode::get_target_type(u32 slot) {
    e_simple_target_type target_type = simple_target_type_none;

    switch (slot) {
        case self_target_slot:
            if (get_self_target())
                return get_self_target()->get_type();

            break;

        case player_target_slot:
            return simple_target_type_entity;

        case collection_slot: {
            ai_simple_target_collection* collection = target_collection;

            if (collection->prune() &&
                collection->current >= 0 &&
                collection->targets[collection->current])

                return collection->get_type();

            break;
        }

        default:
            if (slot < target_count && targets[slot]) {
                target_type = targets[slot]->get_type();

                if (target_type == simple_target_type_none)
                    clear_target(slot);
            }

            break;
    }

    return target_type;
}

// sub_4E3050
// inlined @ sub_4F9FD0
void ai_basic_inode::clear_target(i32 slot) {
    if (slot < 0)
        return;

    if (slot == collection_slot)
        ((void (__thiscall*)(ai_simple_target_collection*))target_collection->vtable[6])(target_collection); // slot 6, clear
    else if (ai_simple_target* target = targets[slot]) {
        target->destroy();

        targets[slot] = nullptr;
    }
}

// sub_4F24E0
ai_simple_target* ai_basic_inode::get_target(u32 slot) {
    if (slot == self_target_slot)
        return get_self_target();

    if (slot == player_target_slot)
        return get_player_target();

    if (slot == collection_slot && target_collection)
        return target_collection;

    get_target_type(slot);

    return targets[slot];
}

// sub_4F2620
// inlined @ sub_4F9FD0
arch_base* ai_basic_inode::get_target_entity(u32 slot) {
    switch (slot) {
        case self_target_slot:
            if (!get_self_target())
                return nullptr;

            return ((ai_simple_target_entity*)get_self_target())->resolve();

        case player_target_slot:
            return get_player_target()->resolve();

        case collection_slot: {
            ai_simple_target* target = target_collection->current >= 0 ?
                target_collection->targets[target_collection->current] : nullptr;

            if (target->get_type() == simple_target_type_entity)
                return ((ai_simple_target_entity*)target)->resolve();

            return nullptr;
        }
    }

    if (get_target_type(slot) != simple_target_type_entity)
        return nullptr;

    return ((ai_simple_target_entity*)targets[slot])->resolve();
}

// sub_4F26E0
bool ai_basic_inode::is_target_valid(u32 slot) {
    if (get_target_type(slot) != simple_target_type_entity)
        return false;

    return get_target_entity(slot)->is_an_actor();
}

// sub_4F2260
void ai_basic_inode::set_target_entity(u32 slot, arch_base* target) { // an entity target is reused, any other replaced
    if (slot >= self_target_slot)
        return;

    // slots past the array land on the members after it, the collection first
    const e_simple_target_type target_type = get_target_type(slot);

    if (target_type == simple_target_type_entity) {
        ((ai_simple_target_entity*)targets[slot])->target = target->my_handle;

        return;
    }

    if (target_type != simple_target_type_none && targets[slot])
        targets[slot]->destroy();

    void* memory = mm::allocate(sizeof(ai_simple_target_entity));

    targets[slot] = memory ? new (memory) ai_simple_target_entity(target->my_handle) : nullptr;
}

// sub_4F22F0
void ai_basic_inode::set_target_location(u32 slot, vector3d location) { // a location target is reused, any other replaced
    if (slot >= self_target_slot)
        return;

    const e_simple_target_type target_type = get_target_type(slot);

    if (target_type == simple_target_type_location) {
        ((ai_simple_target_location*)targets[slot])->point = location;

        return;
    }

    if (target_type != simple_target_type_none && targets[slot])
        targets[slot]->destroy();

    void* memory = mm::allocate(sizeof(ai_simple_target_location));

    targets[slot] = memory ? new (memory) ai_simple_target_location(location) : nullptr;
}

// sub_4E5A30
ai_simple_target* ai_basic_inode::get_self_target() {
    if (!get_actor()->is_alive()) {
        if (!self_target)
            return nullptr;

        self_target->destroy();
        self_target = nullptr;

        return self_target;
    }

    if (self_target)
        return self_target;

    void* memory = mm::allocate(sizeof(ai_simple_target_entity));

    self_target = memory ? new (memory) ai_simple_target_entity(get_actor()->my_handle) : nullptr;

    return self_target;
}

// sub_4E5620
ai_simple_target_entity* ai_basic_inode::get_player_target() {
    if (!references::player_target.read()) {
        void* memory = mm::allocate(sizeof(ai_simple_target_entity));

        references::player_target.write(memory ?
            new (memory) ai_simple_target_entity(references::g_world_ptr.read()->get_hero_ptr()->my_handle) : nullptr);
    }

    return references::player_target.read();
}

// sub_4E2F50
// inlined @ sub_4E5A00
void ai_basic_inode::set_machine(u32 slot, slim::slim_machine* machine) {
    if (slot >= machine_count)
        return;

    machines[slot]         = machine;
    machine_changed[slot] |= 1;
}

// sub_4E5A00
void ai_basic_inode::set_machine(u32 slot, const char* name) {
    set_machine(slot, (slim::slim_machine*)retail::sub_99C0C0(name)); // slim_machine::find_by_name
}

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

// sub_4CB190
void treyarch::rvb_set_target(arch_base* the_entity, u32 slot, arch_base* target) {
    if (ai_basic_inode* inode = get_ai_basic_inode(the_entity))
        inode->set_target_entity(slot, target);
}

// sub_4CB1C0
void treyarch::rvb_set_target(arch_base* the_entity, u32 slot, vector3d location) {
    if (slot == 2 || slot == 16)
        return;

    if (ai_basic_inode* inode = get_ai_basic_inode(the_entity))
        inode->set_target_location(slot, location);
}

// sub_4CB200
void treyarch::rvb_clear_target(arch_base* the_entity, i32 slot) {
    if (ai_basic_inode* inode = get_ai_basic_inode(the_entity))
        inode->clear_target(slot);
}

// sub_4CB220
e_simple_target_type treyarch::rvb_has_target(arch_base* the_entity, u32 slot) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    return inode ? inode->get_target_type(slot) : simple_target_type_none;
}

// sub_4CB420
void treyarch::rvb_set_machine(arch_base* the_entity, u32 slot, const char* name) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (inode && inode->get_flag(ai_flag_is_slim_actor))
        inode->set_machine(slot, name);
}

// sub_4CB460
void treyarch::rvb_clear_machine(arch_base* the_entity, u32 slot) {
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (inode && inode->get_flag(ai_flag_is_slim_actor))
        inode->set_machine(slot, (slim::slim_machine*)nullptr);
}

// sub_4CB950
i32 treyarch::get_car_seat(arch_base* the_entity) { // a slim actor's "actual_car_seat" var, anyone else's car inode seat
    ai_basic_inode* inode = get_ai_basic_inode(the_entity);

    if (inode && inode->get_flag(ai_flag_is_slim_actor)) {
        if (inode->is_inside_car()) {
            string_hash key;
            key.initialize(mash::ALLOCATED, nullptr, 0x2E1F03FB);

            return (i32)inode->get_var_float(&key, nullptr);
        }
    } else if (ai_core* core = ((actor*)the_entity)->get_ai_core()) {
        auto car = (ai_car_inode*)core->get_info_node(info_node_type_ai_car);

        if (car && car->is_in_car)
            return car->selected_seat;
    }

    return -1;
}

// sub_4C4B30
void treyarch::rvb_set_speed_modifier(arch_base* the_entity, f32 modifier) {
    if (!the_entity->is_a_vehicle())
        return;

    if (ai_vehicle* ai = ((vehicle*)the_entity)->my_ai)
        ai->m_inode->speed_modifier = modifier;
}

// sub_4C7590
bool treyarch::rvb_is_crawling(arch_base* the_entity) {
    if (!the_entity->is_an_actor())
        return false;

    auto the_actor = (actor*)the_entity;

    if (!retail::sub_601C50((u32*)the_actor)) {
        if (!the_actor->get_ai_core())
            return false;

        auto swift_surface_layer = (loco_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_swift_surface_layer);

        return swift_surface_layer && (swift_surface_layer->unk_0ac & 0x20);
    }

    if (references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->crawling() ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_102()  ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_103()  ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_104()  ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_105()  ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_106()  ||
        references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_138())

        return true;

    info_node* als_node = the_actor->get_ai_core()->get_info_node(info_node_type_als);

    if (!als_node)
        return false;

    auto move = (u8*)retail::sub_44F330((u32**)als_node, 0); // als_inode: the current combo move

    return move && (*(u32*)(move + 0x10) & 0x8000);
}

// sub_4C77E0
arch_base_vhandle treyarch::rvb_get_animation_coordinator(arch_base* the_entity) {
    if (!the_entity || !the_entity->is_an_actor() || !((actor*)the_entity)->get_ai_core())
        return arch_base_vhandle();

    auto als_node = (als_inode*)((actor*)the_entity)->get_ai_core()->get_info_node(info_node_type_als);

    if (!als_node)
        return arch_base_vhandle();

    auto the_als = (als::animation_logic_system_interface*)als_node->my_als;

    if (!the_als->has_coordinator())
        return arch_base_vhandle();

    return the_als->get_coordinator()->get_actor()->my_handle;
}
