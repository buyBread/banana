#pragma once

#include "treyarch/chuck/vm/so_data_block.hh"
#include "treyarch/chuck/vm/vm_simple_list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_function;
    class script_instance;
    class script_object;
    class vm_thread;

    enum e_script_instance_flags : u32 {
        script_instance_flag_run_called         = 0x01,
        script_instance_flag_auto_destruct      = 0x02, // AUTODEST; collected by the next object run once threadless
        script_instance_flag_small_stack        = 0x04, // 128-byte thread stacks
        script_instance_flag_large_stack        = 0x08, // 512-byte thread stacks
        script_instance_flag_running_destructor = 0x10,
        script_instance_flag_running_callbacks  = 0x20
    };

    enum e_script_instance_callback_reason : i32 {
        script_instance_callback_reason_instance_about_to_die = 0,
        script_instance_callback_reason_thread_about_to_die   = 1
    };

    using script_instance_callback_t = void(*)(e_script_instance_callback_reason reason,
                                               script_instance*                  si,
                                               vm_thread*                        vmt,
                                               void*                             user_data);

    // WoS ordinals; SM3's list differs (it had dynamic arrays at 11)
    enum e_script_garbage_collection_type : u32 {
        script_garbage_collection_dynamic_array = 6 // unlisted by vm_dynamic_array_manager when an array dies
    };

    // SM3's element plus the owning-list backpointer every retail list element carries
    struct garbage_collection_element {
        u32                                          element;
        vm_simple_list<garbage_collection_element*>* list;
        garbage_collection_element*                  vm_simple_list_previous;
        garbage_collection_element*                  vm_simple_list_next;
    };

    using script_instance_created_callback_t   = void(*)(script_instance* inst);
    using script_instance_destroyed_callback_t = void(*)(script_instance* inst);

    namespace references {
        // set_script_instance_callbacks (sub_A1DF80) from sub_843300: nothing, and the game's shadow teardown (sub_825DE0)
        inline util::memory_reference<script_instance_created_callback_t>   script_instance_created_callback   { 0x01124BA8 };
        inline util::memory_reference<script_instance_destroyed_callback_t> script_instance_destroyed_callback { 0x01124BAC };

        inline util::memory_reference<memory::fixed_pool> callback_node_pool              { 0x01124C00 };
        inline util::memory_reference<memory::fixed_pool> garbage_collection_element_pool { 0x01125138 };
    } // references

    // add: sub_A1DDC0, remove: sub_A1E160; pushed at the head, retail keeps no back link
    struct script_instance_callback_node {
        void*                          user_data;
        script_instance_callback_node* next;
    };

    class script_instance {

    public:
        e_script_instance_flags           flags;
        string_hash                       name;          // "__singleton" for first-run singletons
        so_data_block                     data;
        vm_simple_list<vm_thread*>        threads;
        script_object*                    parent;
        script_instance_callback_t        callback;
        script_instance_callback_node*    callback_nodes;

        vm_simple_list<garbage_collection_element*>* allocated_stuff; // one list per garbage-collection type

        u32                               unk_34;
        void*                             client_space;  // the game's script_instance_shadow (sub_825600)
        u32                               unk_3c;
        engine_recursive_lock             thread_lock;
        engine_recursive_lock             allocated_stuff_lock;
        vm_simple_list<script_instance*>* instance_list; // the parent's `instances`
        script_instance*                  vm_simple_list_previous;
        script_instance*                  vm_simple_list_next;
        u32                               unk_6c;

        void register_callback(script_instance_callback_t requested_callback, void* user_data);
        void unregister_callback(void* user_data);

        static void set_script_instance_callbacks(script_instance_created_callback_t   created_callback,
                                                  script_instance_destroyed_callback_t destroyed_callback);

        vm_thread* add_thread(const script_function* ex, void* user_data, u32 stack_size);
        vm_thread* add_thread(const script_function* ex,
                              const void*            arguments,
                              i32                    argument_size,
                              void*                  user_data,
                              u32                    stack_size);

        void remove_allocated_stuff(e_script_garbage_collection_type type, u32 stuff);
    };

    ASSERT_SIZEOF  (script_instance_callback_node, 0x08);

    ASSERT_SIZEOF  (garbage_collection_element,                          0x10);
    ASSERT_OFFSETOF(garbage_collection_element, list,                    0x04);
    ASSERT_OFFSETOF(garbage_collection_element, vm_simple_list_previous, 0x08);
    ASSERT_OFFSETOF(garbage_collection_element, vm_simple_list_next,     0x0C);

    ASSERT_SIZEOF  (script_instance,                          0x70);
    ASSERT_OFFSETOF(script_instance, flags,                   0x00);
    ASSERT_OFFSETOF(script_instance, name,                    0x04);
    ASSERT_OFFSETOF(script_instance, data,                    0x08);
    ASSERT_OFFSETOF(script_instance, threads,                 0x18);
    ASSERT_OFFSETOF(script_instance, parent,                  0x24);
    ASSERT_OFFSETOF(script_instance, callback,                0x28);
    ASSERT_OFFSETOF(script_instance, callback_nodes,          0x2C);
    ASSERT_OFFSETOF(script_instance, allocated_stuff,         0x30);
    ASSERT_OFFSETOF(script_instance, client_space,            0x38);
    ASSERT_OFFSETOF(script_instance, thread_lock,             0x40);
    ASSERT_OFFSETOF(script_instance, allocated_stuff_lock,    0x50);
    ASSERT_OFFSETOF(script_instance, instance_list,           0x60);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_previous, 0x64);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_next,     0x68);
}}} // treyarch::chuck::vm
