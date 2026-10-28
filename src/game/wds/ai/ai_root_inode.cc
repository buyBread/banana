#include <new>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/wds/ai/ai_root_inode.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_4E53E0
f32 ai_root_inode::get_var_float(const string_hash* key, bool* missing) { // a miss answers 0 when asked about, the generic actor defaults otherwise
    bool exists = false;

    if (missing)
        *missing = false;

    const param_block* block = var_block ? var_block : &my_param_block;

    const f32 value = block->get_optional_pb_float(*key, 0.0f, &exists);

    if (exists)
        return value;

    if (missing) {
        *missing = true;

        return 0.0f;
    }

    auto defaults = (u32*)retail::sub_4E2550((u32*)this, 1); // get_generic_actor_defaults

    return (f32)retail::sub_4C4B60(defaults, (u32*)key, nullptr); // json_def_values::get_float
}

// sub_4E2390
void ai_root_inode::set_var_float(const string_hash* key, f32 value) {
    if (!key->source_hash_code)
        return;

    param_block* block = var_block ? var_block : &my_param_block;

    block->set_pb_float(*key, value);
}

// sub_4F9020
void ai_root_inode::set_flag(i32 flag, bool value) {
    retail::sub_4F8F10((u8*)flags, flag, value); // ai_flag_store::set
}

// sub_4F9030
bool ai_root_inode::get_flag(i32 flag) {
    return retail::sub_4F8FA0((u8*)flags, flag); // ai_flag_store::get
}

// sub_4E2670
void ai_fd_inode::clear_target(i32 index) {
    if (ai_simple_target* target = targets[index]) {
        target->destroy();

        targets[index] = nullptr;
    }
}

// sub_4E26A0
e_simple_target_type ai_fd_inode::get_target_type(i32 index) {
    ai_simple_target* target = targets[index];

    if (!target)
        return simple_target_type_none;

    const e_simple_target_type type = target->get_type();

    if (type == simple_target_type_none)
        clear_target(index);

    return type;
}

// sub_4E5940
ai_simple_target* ai_fd_inode::get_live_target(i32 index) {
    ai_simple_target* target = targets[index];

    if (target && target->get_type() == simple_target_type_none)
        clear_target(index);

    return targets[index];
}

// sub_4E57E0
void ai_fd_inode::set_target_entity(i32 index, arch_base* target) { // an entity target is reused, any other replaced
    if ((u32)index >= target_count)
        return;

    const e_simple_target_type type = get_target_type(index);

    if (type == simple_target_type_entity) {
        ((ai_simple_target_entity*)targets[index])->target = target->my_handle;

        return;
    }

    if (type != simple_target_type_none && targets[index])
        targets[index]->destroy();

    void* memory = mm::allocate(sizeof(ai_simple_target_entity));

    targets[index] = memory ? new (memory) ai_simple_target_entity(target->my_handle) : nullptr;
}

// sub_4E5890
void ai_fd_inode::set_target_point(i32 index, vector3d point) { // a location target is reused, any other replaced
    if ((u32)index >= target_count)
        return;

    const e_simple_target_type type = get_target_type(index);

    if (type == simple_target_type_location) {
        ((ai_simple_target_location*)targets[index])->point = point;

        return;
    }

    if (type != simple_target_type_none && targets[index])
        targets[index]->destroy();

    void* memory = mm::allocate(sizeof(ai_simple_target_location));

    targets[index] = memory ? new (memory) ai_simple_target_location(point) : nullptr;
}
