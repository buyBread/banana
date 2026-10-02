#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// inlined into add_reference and release_reference
i32 vm_dynamic_array_manager::find_slot(const chuck_dynamic_array_t* array) {
    i32 bucket_count = (i32)slot_buckets->size();

    for (i32 bucket = 0; bucket < bucket_count; ++bucket) {
        if (slot_buckets->begin()[bucket] == 0xFFFF)
            continue;

        for (i32 slot = bucket * 16; slot < bucket * 16 + 16; ++slot) {
            if ((*array_pool)[slot].array == array)
                return slot;
        }
    }

    return -1;
}

// sub_A18420
void vm_dynamic_array_manager::add_reference(chuck_dynamic_array_t* array) {
    if (!array)
        return;

    engine_lock_scope scope(references::dynamic_array_lock.read());

    i32 slot = find_slot(array);

    if (slot >= 0)
        ++(*array_pool)[slot].ref_count;
}

// sub_A18500
bool vm_dynamic_array_manager::find_by_data(const void* data, chuck_dynamic_array_t** found) {
    if (!data)
        return false;

    engine_lock_scope scope(references::dynamic_array_lock.read());

    i32 bucket_count = (i32)slot_buckets->size();

    for (i32 bucket = 0; bucket < bucket_count; ++bucket) {
        if (slot_buckets->begin()[bucket] == 0xFFFF)
            continue;

        for (i32 slot = bucket * 16; slot < bucket * 16 + 16; ++slot) {
            chuck_dynamic_array_t* candidate = (*array_pool)[slot].array;

            if (candidate->size() && data == candidate->begin()) {
                if (found)
                    *found = candidate;

                return true;
            }
        }
    }

    return false;
}

// sub_A18320
void vm_dynamic_array_manager::remove_reference(chuck_dynamic_array_t* array) {
    if (!array)
        return;

    engine_lock_scope scope(references::dynamic_array_lock.read());

    release_reference(array, nullptr, vm_dynamic_array_release_plain);
}

// sub_A18780
void vm_dynamic_array_manager::remove_string_reference(const void* string) {
    if (!string)
        return;

    chuck_dynamic_array_t* owner;

    if (find_by_data(string, &owner))
        remove_reference(owner);
}

// sub_A190A0
void vm_dynamic_array_manager::remove_string_array_reference(chuck_dynamic_array_t* array) {
    if (!array)
        return;

    engine_lock_scope scope(references::dynamic_array_lock.read());

    release_reference(array, nullptr, vm_dynamic_array_release_strings);
}

// sub_A19120
void vm_dynamic_array_manager::remove_instance_array_reference(script_instance*       context,
                                                               chuck_dynamic_array_t* array) {

    if (!array)
        return;

    engine_lock_scope scope(references::dynamic_array_lock.read());

    release_reference(array, context, vm_dynamic_array_release_erase);
}

// sub_A18EA0
void vm_dynamic_array_manager::release_reference(chuck_dynamic_array_t*          array,
                                                 script_instance*                context,
                                                 e_vm_dynamic_array_release_mode mode) {

    i32 slot = find_slot(array);

    if (slot == -1)
        return;

    vm_dynamic_array_slot &entry = (*array_pool)[slot];

    if (--entry.ref_count)
        return;

    switch (mode) {
        case vm_dynamic_array_release_strings:
            release_string_elements(array);
            break;

        case vm_dynamic_array_release_erase:
            if (array)
                array->erase(array->begin(), array->end());
            break;

        default:
            break;
    }

    array->erase(array->begin(), array->end());
    array->destroy();

    if (entry.owner && !unk_08)
        entry.owner->remove_allocated_stuff(script_garbage_collection_dynamic_array, (u32)array);

    slot_buckets->begin()[slot / 16] |= (u16)(1 << (slot % 16));
}

// sub_A18C40
void vm_dynamic_array_manager::release_string_elements(chuck_dynamic_array_t* array) {
    if (!array)
        return;

    for (u32 index = 0; index < array->size(); ++index) {
        const void* string = *(const void**)(*array)[index].stuff;

        chuck_dynamic_array_t* owner;

        if (string && find_by_data(string, &owner))
            remove_reference(owner);
    }

    array->clear();
}
