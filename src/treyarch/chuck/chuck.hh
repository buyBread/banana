#pragma once

#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/script_library/script_library_registry.hh"
#include "util/memory_reference.hh"
#include "util/macros/sanity_assert.hh"

/*
    current implementation is a rudimentary ABI skeleton that slots into specific addresses.
    also, need to move all the ABI asserts, but really i don't want to touch this...
    unless i begin bootstrapping the VM or SL
*/

namespace treyarch { namespace chuck {
    
    namespace vm { namespace references {
        inline util::memory_reference<script_manager*>
            script_manager { 0x011248E0 };
    }} // vm::script_library::references

    namespace script_library { namespace references {
        inline util::memory_reference<script_library_registry*>
            script_library_registry { 0x01124C64 };
    }} // script_library::references

    /*
        sizeofs
    */

    ASSERT_SIZEOF(vm::vm_reference_descriptor, 0x04);
    ASSERT_SIZEOF(vm::vm_flow_frame,           0x0C);
    ASSERT_SIZEOF(vm::vm_thread,               0x84);
    ASSERT_SIZEOF(vm::vm_argument,             0x0C);

    ASSERT_SIZEOF(vm::script_function,               0x2C);
    ASSERT_SIZEOF(vm::script_executable_tree_node,   0x14);
    ASSERT_SIZEOF(vm::script_executable_tree,        0x0C);
    ASSERT_SIZEOF(vm::script_executable,             0x6C);
    ASSERT_SIZEOF(vm::script_object,                 0x5C);
    ASSERT_SIZEOF(vm::script_instance,               0x70);
    ASSERT_SIZEOF(vm::script_instance_callback_node, 0x08);
    ASSERT_SIZEOF(vm::script_manager,                0x98);
    ASSERT_SIZEOF(vm::script_variable_block,         0x10);
    ASSERT_SIZEOF(vm::script_var_address_entry,      0x08);
    ASSERT_SIZEOF(vm::script_var_debug_info,         0x04);
    ASSERT_SIZEOF(vm::script_var_container,          0x30);

    ASSERT_SIZEOF(script_library::script_library_registry,        0x04);
    ASSERT_SIZEOF(script_library::script_library_function,        0x04);
    ASSERT_SIZEOF(script_library::script_library_function_vector, 0x10);
    ASSERT_SIZEOF(script_library::script_library_class,           0x10);
    ASSERT_SIZEOF(script_library::script_library_class_vector,    0x10);

    /*
        offsetofs
    */

    ASSERT_OFFSETOF(vm::vm_flow_frame, previous_current_instance, 0x04);

    ASSERT_OFFSETOF(vm::vm_thread, entry_function,          0x04);
    ASSERT_OFFSETOF(vm::vm_thread, stack,                   0x0C);
    ASSERT_OFFSETOF(vm::vm_thread, pc,                      0x20);
    ASSERT_OFFSETOF(vm::vm_thread, next_push_pop_modifier,  0x24);
    ASSERT_OFFSETOF(vm::vm_thread, next_push_pop_arg,       0x28);
    ASSERT_OFFSETOF(vm::vm_thread, next_push_pop_subscript, 0x34);
    ASSERT_OFFSETOF(vm::vm_thread, flow_frame,              0x38);
    ASSERT_OFFSETOF(vm::vm_thread, current_opcode,          0x3C);
    ASSERT_OFFSETOF(vm::vm_thread, current_arg_type,        0x40);
    ASSERT_OFFSETOF(vm::vm_thread, current_arg,             0x44);
    ASSERT_OFFSETOF(vm::vm_thread, current_dsize,           0x50);
    ASSERT_OFFSETOF(vm::vm_thread, current_instance,        0x54);
    ASSERT_OFFSETOF(vm::vm_thread, unk_64,                  0x64);
    ASSERT_OFFSETOF(vm::vm_thread, local_references,        0x6C);
    ASSERT_OFFSETOF(vm::vm_thread, id,                      0x68);
    ASSERT_OFFSETOF(vm::vm_thread, thread_list_owner,       0x78);
    ASSERT_OFFSETOF(vm::vm_thread, next,                    0x80);

    ASSERT_OFFSETOF(vm::script_var_container, script_var_block,      0x04);
    ASSERT_OFFSETOF(vm::script_var_container, script_var_to_address, 0x14);
    ASSERT_OFFSETOF(vm::script_var_container, debug_info,            0x28);
    ASSERT_OFFSETOF(vm::script_var_container, flags,                 0x2C);

    ASSERT_OFFSETOF(vm::script_function, reference_descriptor_count, 0x04);
    ASSERT_OFFSETOF(vm::script_function, reference_descriptors,      0x08);
    ASSERT_OFFSETOF(vm::script_function, signature_hash,             0x10);
    ASSERT_OFFSETOF(vm::script_function, object,                     0x1C);
    ASSERT_OFFSETOF(vm::script_function, code,                       0x20);
    ASSERT_OFFSETOF(vm::script_function, stack_metadata,             0x24);
    ASSERT_OFFSETOF(vm::script_function, flags,                      0x2A);

    ASSERT_OFFSETOF(vm::script_object, function_count,     0x18);
    ASSERT_OFFSETOF(vm::script_object, functions,          0x1C);
    ASSERT_OFFSETOF(vm::script_object, first_instance,     0x48);
    ASSERT_OFFSETOF(vm::script_object, instance_lock,      0x58);
    ASSERT_OFFSETOF(vm::script_object, global_instance,    0x0C);
    ASSERT_OFFSETOF(vm::script_object, instance_data_size, 0x10);
    ASSERT_OFFSETOF(vm::script_object, constructor_index,  0x28);
    ASSERT_OFFSETOF(vm::script_object, destructor_index,   0x2C);
    ASSERT_OFFSETOF(vm::script_object, last_instance,      0x4C);
    ASSERT_OFFSETOF(vm::script_object, instance_count,     0x50);

    ASSERT_OFFSETOF(vm::script_executable, object_count,              0x18);
    ASSERT_OFFSETOF(vm::script_executable, objects,                   0x1C);
    ASSERT_OFFSETOF(vm::script_executable, code,                      0x54);
    ASSERT_OFFSETOF(vm::script_executable, flags,                     0x64);
    ASSERT_OFFSETOF(vm::script_executable, permanent_string_count,    0x44);
    ASSERT_OFFSETOF(vm::script_executable, permanent_strings,         0x48);
    ASSERT_OFFSETOF(vm::script_executable, permanent_string_capacity, 0x4C);

    ASSERT_OFFSETOF(vm::script_executable_tree_node, entry,  0x0C);
    ASSERT_OFFSETOF(vm::script_executable_tree_node, is_nil, 0x11);

    ASSERT_OFFSETOF(vm::script_executable_tree, head,  0x04);
    ASSERT_OFFSETOF(vm::script_executable_tree, count, 0x08);

    ASSERT_OFFSETOF(vm::script_instance, first_thread,        0x18);
    ASSERT_OFFSETOF(vm::script_instance, object,              0x24);
    ASSERT_OFFSETOF(vm::script_instance, thread_lock,         0x40);
    ASSERT_OFFSETOF(vm::script_instance, next,                0x68);
    ASSERT_OFFSETOF(vm::script_instance, script_variables,    0x08);
    ASSERT_OFFSETOF(vm::script_instance, instance_list_owner, 0x60);
    ASSERT_OFFSETOF(vm::script_instance, previous,            0x64);
    ASSERT_OFFSETOF(vm::script_instance, lifecycle_callback,  0x28);
    ASSERT_OFFSETOF(vm::script_instance, callback_list_head,  0x2C);

    ASSERT_OFFSETOF(vm::script_manager, loaded_executables,            0x08);
    ASSERT_OFFSETOF(vm::script_manager, lifecycle_callback,            0x58);
    ASSERT_OFFSETOF(vm::script_manager, loaded_executables_lock,       0x78);
    ASSERT_OFFSETOF(vm::script_manager, latest_tick_argument,          0x04);
    ASSERT_OFFSETOF(vm::script_manager, master_script,                 0x0C);
    ASSERT_OFFSETOF(vm::script_manager, game_variable_container,       0x10);
    ASSERT_OFFSETOF(vm::script_manager, shared_variable_container,     0x14);
    ASSERT_OFFSETOF(vm::script_manager, executables_pending_first_run, 0x18);
    ASSERT_OFFSETOF(vm::script_manager, callback_lock,                 0x88);

    ASSERT_OFFSETOF(script_library::script_library_registry, m_classes, 0x00);

    ASSERT_OFFSETOF(script_library::script_library_function_vector, first,        0x04);
    ASSERT_OFFSETOF(script_library::script_library_function_vector, last,         0x08);
    ASSERT_OFFSETOF(script_library::script_library_function_vector, capacity_end, 0x0C);

    ASSERT_OFFSETOF(script_library::script_library_class, m_value_size,  0x04);
    ASSERT_OFFSETOF(script_library::script_library_class, m_functions,   0x08);
    ASSERT_OFFSETOF(script_library::script_library_class, m_parent_name, 0x0C);
    
    ASSERT_OFFSETOF(script_library::script_library_class_vector, first,        0x04);
    ASSERT_OFFSETOF(script_library::script_library_class_vector, last,         0x08);
    ASSERT_OFFSETOF(script_library::script_library_class_vector, capacity_end, 0x0C);
}}