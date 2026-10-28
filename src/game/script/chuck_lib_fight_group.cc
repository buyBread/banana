#include "retail.hh"
#include "game/arch_base.hh"
#include "game/hash/algo.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_fight_group.hh"
#include "game/wds/ai/ai_root_inode.hh"
#include "game/wds/ai/fight_group.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

bool slf__activate_fight_group__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878240)(this, stack);
}

// sub_8782F0
bool slf__fight_group__pause_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_fight_group())
        self->flags |= fight_group_flag_paused;

    return true;
}

// sub_878330
bool slf__fight_group__unpause_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_fight_group())
        self->flags &= ~fight_group_flag_paused;

    return true;
}

// sub_878370
bool slf__fight_group__release_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_fight_group())
        retail::sub_4DC390((u32*)self); // fight_group::release_fight_group

    return true;
}

// sub_8783B0
bool slf__fight_group__add_actor_to_fight_group__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto actor = arch_base_vhandle(stack.local_uint(0x04));
    auto flags = stack.local_num(0x08);

    // an actor that doesn't resolve faults
    if (actor.resolve()->is_an_actor() && self && self->is_a_fight_group())
        retail::sub_4DDD60((u32*)self, (i32)actor.value, (i32)flags); // fight_group::add_actor_to_fight_group

    return true;
}

// sub_878410
bool slf__fight_group__set_actor_group_flags__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto actor = arch_base_vhandle(stack.local_uint(0x04));
    auto flags = stack.local_num(0x08);

    // same
    if (actor.resolve()->is_an_actor() && self && self->is_a_fight_group())
        self->set_actor_group_flags(actor, (u32)(i32)flags);

    return true;
}

// sub_878470
bool slf__fight_group__clear_actor_group_flags__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto actor = arch_base_vhandle(stack.local_uint(0x04));
    auto flags = stack.local_num(0x08);

    // same
    if (actor.resolve()->is_an_actor() && self && self->is_a_fight_group())
        self->clear_actor_group_flags(actor, (u32)(i32)flags);

    return true;
}

// sub_8784D0
bool slf__fight_group__get_group_count__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_fight_group() ? (f32)(i32)self->members.size() : -1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_883770
bool slf__fight_group__rvb_set_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    // a name the map doesn't know sets flag -1
    if (self && self->is_a_fight_group() && self->fd_inode)
        self->fd_inode->set_flag(find_ai_flag(hash::djb2(name)), true);

    return true;
}

// sub_8837D0
bool slf__fight_group__rvb_clear_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    if (self && self->is_a_fight_group() && self->fd_inode)
        self->fd_inode->set_flag(find_ai_flag(hash::djb2(name)), false);

    return true;
}

// sub_883830
bool slf__fight_group__rvb_get_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_fight_group() && self->fd_inode) {
        string_hash key;
        key.initialize(mash::ALLOCATED, name);

        result = self->fd_inode->get_flag(find_ai_flag(key.source_hash_code)) ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878570
bool slf__fight_group__rvb_set_var__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = stack.local_num(0x08);

    if (self && self->is_a_fight_group() && self->fd_inode) {
        string_hash key;
        key.initialize(mash::ALLOCATED, name);

        self->fd_inode->set_var_float(&key, value);
    }

    return true;
}

// sub_8785D0
bool slf__fight_group__rvb_get_var__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_fight_group() && self->fd_inode) {
        string_hash key;
        key.initialize(mash::ALLOCATED, name);

        result = self->fd_inode->get_var_float(&key, nullptr);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878670
bool slf__fight_group__rvb_set_target__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index  = stack.local_num(0x04);
    auto target = arch_base_vhandle(stack.local_uint(0x08));

    // the target resolves once for the test and again for the call
    if (self && self->is_a_fight_group() && target.resolve() && self->fd_inode)
        self->fd_inode->set_target_entity((i32)index, target.resolve());

    return true;
}

// sub_8786D0
bool slf__fight_group__rvb_set_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);
    auto point = (const vector3d*)&stack.local_uint(0x08);

    if (self && self->is_a_fight_group() && self->fd_inode)
        self->fd_inode->set_target_point((i32)index, *point);

    return true;
}

// sub_878730
bool slf__fight_group__rvb_has_target__num__t::operator()(chuck::vm::vm_stack &stack) { // the target's type as a num
    stack.sp -= 0x08;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    f32 result = self && self->is_a_fight_group() && self->fd_inode ? (f32)self->fd_inode->get_target_type((i32)index) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8787B0
bool slf__fight_group__rvb_get_target_entity__num__t::operator()(chuck::vm::vm_stack &stack) { // a location answers a new marker placed there, every call
    stack.sp -= 0x08;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    u32 result = 0;

    if (self && self->is_a_fight_group() && self->fd_inode) {
        ai_fd_inode* inode = self->fd_inode;

        inode->get_target_type((i32)index);

        if (ai_simple_target* target = inode->get_live_target((i32)index)) {
            if (target->get_type() == simple_target_type_entity)
                result = ((ai_simple_target_entity*)target)->resolve()->my_handle.value; // a gone entity faults
            else if (target->get_type() == simple_target_type_location) {
                auto marker = (arch_base*)retail::sub_804740((u32*)&references::g_world_ptr.read()->ent_mgr,
                                                             (i32)hash::djb2("inviso_nullo"),
                                                             nullptr); // wds_entity_manager::spawn_entity_autoname

                vector3d position = target->get_position(string_hash());

                // so does a failed spawn
                retail::sub_626CA0((u32*)marker, (u32*)&position); // entity_base::set_rel_position

                if (marker && marker->is_an_entity())
                    result = marker->my_handle.value;
            }
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8788C0
bool slf__fight_group__rvb_get_target_vector__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    vector3d result = references::unk_010f7140.get();

    if (self && self->is_a_fight_group() && self->fd_inode) {
        ai_fd_inode* inode = self->fd_inode;

        inode->get_target_type((i32)index);

        if (ai_simple_target* target = inode->get_live_target((i32)index))
            result = target->get_position(string_hash());
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878990
bool slf__fight_group__rvb_clear_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (fight_group*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    if (self && self->is_a_fight_group() && self->fd_inode)
        self->fd_inode->clear_target((i32)index);

    return true;
}
