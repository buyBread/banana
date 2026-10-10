#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A20400
void script_function::post_un_mash_fixup(script_object* requested_parent) {
    u32 offset = (u32)buffer;

    parent = requested_parent;
    buffer = requested_parent->parent->lookup_sx_code_segment(offset);
}

// sub_A20420
void script_function::finalize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED)
        buffer = nullptr;
}

// sub_A20440
void script_function::add_references(vm_thread*  thread,
                                     const void* block,
                                     u32         block_size,
                                     bool        retain,
                                     bool        track) const {

    vm_dynamic_array_manager* manager = vm_dynamic_array_manager::inst();

    for (u32 index = 0; index < reference_descriptors.size; ++index) {
        const vm_reference_descriptor &descriptor = reference_descriptors.data[index];

        if (descriptor.offset >= block_size) {
            // retail formats "bad offset trying to add tracking info for ref-counted item in '%s'" for a stripped print (nullsub_1)
            fullname.to_string();

            continue;
        }

        void* value = *(void**)((const u8*)block + descriptor.offset);

        switch (descriptor.kind) {
            case vm_reference_kind_dynamic_array:
            case vm_reference_kind_string_dynamic_array:
            case vm_reference_kind_script_instance_dynamic_array:
                if (retain)
                    manager->add_reference((chuck_dynamic_array_t*)value);

                if (track) {
                    u32 mode = descriptor.kind == vm_reference_kind_string_dynamic_array          ? 4 :
                               descriptor.kind == vm_reference_kind_script_instance_dynamic_array ? 8 : 0;

                    thread->add_local_reference(value, mode);
                }

                break;

            // a `str` points into its owning array
            case vm_reference_kind_string: {
                chuck_dynamic_array_t* owner = nullptr;

                if (manager->find_by_data(value, &owner)) {
                    if (retain)
                        manager->add_reference(owner);

                    if (track)
                        thread->add_local_reference(owner, 0);
                }

                break;
            }

            default:
                break;
        }
    }
}

// sub_A20560
void script_function::release_references(const void* block, u32 block_size) const {
    vm_dynamic_array_manager* manager = vm_dynamic_array_manager::inst();

    for (u32 index = 0; index < reference_descriptors.size; ++index) {
        const vm_reference_descriptor &descriptor = reference_descriptors.data[index];

        if (descriptor.offset >= block_size) {
            // retail formats "bad offset trying to remove reference for ref-counted item in '%s'" for a stripped print (nullsub_1)
            fullname.to_string();

            continue;
        }

        void* value = *(void**)((const u8*)block + descriptor.offset);

        switch (descriptor.kind) {
            case vm_reference_kind_dynamic_array:
                manager->remove_reference((chuck_dynamic_array_t*)value);
                
                break;

            case vm_reference_kind_string_dynamic_array:
                manager->remove_string_array_reference((chuck_dynamic_array_t*)value);

                break;

            case vm_reference_kind_script_instance_dynamic_array:
                manager->remove_instance_array_reference(nullptr, (chuck_dynamic_array_t*)value);

                break;

            case vm_reference_kind_string:
                manager->remove_string_reference(value);

                break;

            default:
                break;
        }
    }
}

// sub_A20650
void script_function::destruct_mashed_class() {
    reference_descriptors.destruct_mashed_class();
}

// sub_A206A0
void script_function::add_thread_references(vm_thread* thread, bool retain, bool track) const {
    add_references(thread, thread->dstack.buffer, parms_stacksize, retain, track);
}
