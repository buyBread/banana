#pragma once

#include "treyarch/chuck/vm/so_data_block.hh"
#include "treyarch/chuck/vm/vm_simple_list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
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

    using script_instance_created_callback_t   = void(*)(script_instance* inst);
    using script_instance_destroyed_callback_t = void(*)(script_instance* inst);

    namespace references {
        // set_script_instance_callbacks (sub_A1DF80) from sub_843300: nothing, and the game's shadow teardown (sub_825DE0)
        inline util::memory_reference<script_instance_created_callback_t>   script_instance_created_callback   { 0x01124BA8 };
        inline util::memory_reference<script_instance_destroyed_callback_t> script_instance_destroyed_callback { 0x01124BAC };
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
        u8                                unk_30[0x08];
        void*                             client_space;  // the game's script_instance_shadow (sub_825600)
        u32                               unk_3c;
        engine_recursive_lock             thread_lock;
        u8                                unk_50[0x10];
        vm_simple_list<script_instance*>* instance_list; // the parent's `instances`
        script_instance*                  vm_simple_list_previous;
        script_instance*                  vm_simple_list_next;
        u32                               unk_6c;
    };

    ASSERT_SIZEOF  (script_instance_callback_node, 0x08);

    ASSERT_SIZEOF  (script_instance,                          0x70);
    ASSERT_OFFSETOF(script_instance, flags,                   0x00);
    ASSERT_OFFSETOF(script_instance, name,                    0x04);
    ASSERT_OFFSETOF(script_instance, data,                    0x08);
    ASSERT_OFFSETOF(script_instance, threads,                 0x18);
    ASSERT_OFFSETOF(script_instance, parent,                  0x24);
    ASSERT_OFFSETOF(script_instance, callback,                0x28);
    ASSERT_OFFSETOF(script_instance, callback_nodes,          0x2C);
    ASSERT_OFFSETOF(script_instance, client_space,            0x38);
    ASSERT_OFFSETOF(script_instance, thread_lock,             0x40);
    ASSERT_OFFSETOF(script_instance, instance_list,           0x60);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_previous, 0x64);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_next,     0x68);
}}} // treyarch::chuck::vm
