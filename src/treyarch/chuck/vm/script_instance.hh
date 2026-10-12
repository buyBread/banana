#pragma once

#include <cstddef>

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

    enum e_script_instance_stack_size : u32 {
        script_instance_stack_size_small,
        script_instance_stack_size_normal,
        script_instance_stack_size_large
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
    // what script library natives list on the creating instance; the names are the natives' script types
    enum e_script_garbage_collection_type : u32 {
        script_garbage_collection_unk_00                 = 0,  // nothing lists it
        script_garbage_collection_entity_tracker         = 1,
        script_garbage_collection_line_info              = 2,
        script_garbage_collection_trigger                = 3,
        script_garbage_collection_entity                 = 4,
        script_garbage_collection_sound_response         = 5,
        script_garbage_collection_dynamic_array          = 6,  // listed when the manager creates an owned array, unlisted when it dies
        script_garbage_collection_generic_event_callback = 7,
        script_garbage_collection_point_of_interest      = 8,
        script_garbage_collection_mutex                  = 9,
        script_garbage_collection_widget_3d              = 10,
        script_garbage_collection_city_life_tracker      = 11,
        script_garbage_collection_finger_of_god          = 12,
        script_garbage_collection_fight_group            = 13,
        script_garbage_collection_obstacle               = 14,
        script_garbage_collection_total_types            = 15
    };

    struct garbage_collection_element {
        u32                                          element;
        vm_simple_list<garbage_collection_element*>* list;
        garbage_collection_element*                  vm_simple_list_previous;
        garbage_collection_element*                  vm_simple_list_next;

        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;
    };

    using stuff_to_delete_list_t = vm_simple_list<garbage_collection_element*>;

    // allocated on the first add_allocated_stuff
    struct script_instance_allocated_stuff_record {
        stuff_to_delete_list_t lists[script_garbage_collection_total_types];

        script_instance_allocated_stuff_record();

        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;
    };

    // installed per type in script_manager; called before the type's list is emptied
    using script_instance_garbage_collection_callback_t = void(*)(script_instance*        si,
                                                                  stuff_to_delete_list_t &stuff_to_delete);

    using script_instance_created_callback_t   = void(*)(script_instance* inst);
    using script_instance_destroyed_callback_t = void(*)(script_instance* inst);

    namespace references {
        // set_script_instance_callbacks (sub_A1DF80) from sub_843300: nothing, and the game's shadow teardown (sub_825DE0)
        inline util::memory_reference<script_instance_created_callback_t>   script_instance_created_callback   { 0x01124BA8 };
        inline util::memory_reference<script_instance_destroyed_callback_t> script_instance_destroyed_callback { 0x01124BAC };

        inline util::memory_reference<memory::fixed_pool> instance_pool                   { 0x01124BB0 };
        inline util::memory_reference<memory::fixed_pool> callback_node_pool              { 0x01124C00 };
        inline util::memory_reference<memory::fixed_pool> allocated_stuff_pool            { 0x011250F0 };
        inline util::memory_reference<memory::fixed_pool> garbage_collection_element_pool { 0x01125138 };
    } // references

    // add: sub_A1DDC0, remove: sub_A1E160; pushed at the head, retail keeps no back link
    struct script_instance_callback_node {
        void*                          user_data;
        script_instance_callback_node* next;

        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;
    };

    class script_instance {

    public:
        e_script_instance_flags                 flags;
        string_hash                             name;           // "__singleton" for first-run singletons
        so_data_block                           data;
        vm_simple_list<vm_thread*>              threads;
        script_object*                          parent;
        script_instance_callback_t              callback;
        script_instance_callback_node*          callback_nodes;

        script_instance_allocated_stuff_record* allocated_stuff;

        i32                                     suspend_count; // run skips the instance while positive, unless told to ignore it
        void*                                   client_space;  // the game's script_instance_shadow (sub_825600)
        u32                                     unk_3c;
        engine_recursive_lock                   thread_lock;
        engine_recursive_lock                   allocated_stuff_lock;
        vm_simple_list<script_instance*>*       list;          // the parent's `instances`
        script_instance*                        vm_simple_list_previous;
        script_instance*                        vm_simple_list_next;
        u32                                     unk_6c;

        // leaves unk_3c, unk_6c and the lock reserves as the pool left them
        script_instance(const char* requested_name, i32 data_size, u32 requested_flags);
       ~script_instance();

        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;

        void set_stack_size(e_script_instance_stack_size stack_size);

        void run(bool ignore_suspended);

        // true once the thread has finished (and been deleted); key_prefix is never read
        bool run_single_thread            (vm_thread* t, bool ignore_suspended, string_hash key_prefix);
        bool run_single_thread_with_return(vm_thread*  t,
                                           bool        ignore_suspended,
                                           void*       ret,
                                           u32         return_value_byte_size,
                                           string_hash key_prefix);

        // false when nothing may run yet; otherwise it keeps running the thread until it finishes
        bool run_single_thread_until_finished(vm_thread* t, bool ignore_suspended, string_hash key_prefix);

        void kill_thread(const script_function* ex, const vm_thread* ignore_thread);
        void kill_thread(vm_thread* thread_to_kill);
        bool massacre_threads_by_function(const script_function* ex, const vm_thread* ignore_thread);
        bool massacre_threads_by_thread(vm_thread* thread_to_massacre, const vm_thread* ignore_thread);

        void enable_auto_destruct();

        void suspend();
        void unsuspend(); // never below zero

        bool thread_exists(u32 thread_id);
        bool thread_exists(vm_thread* thread, u32 thread_id);

        void run_callbacks(e_script_instance_callback_reason reason, vm_thread* vmt);

        void register_callback(script_instance_callback_t requested_callback, void* user_data);
        void unregister_callback(void* user_data);

        static void set_script_instance_callbacks(script_instance_created_callback_t   created_callback,
                                                  script_instance_destroyed_callback_t destroyed_callback);

        vm_thread* add_thread(const script_function* ex, void* user_data, u32 stack_size);
        vm_thread* add_thread(const script_function* ex,
                              const void*            arguments,
                                    i32              argument_size,
                                    void*            user_data,
                                    u32              stack_size);

        void add_allocated_stuff   (e_script_garbage_collection_type type, u32 stuff);
        void remove_allocated_stuff(e_script_garbage_collection_type type, u32 stuff);

        void do_garbage_collection();

        // empties one garbage-collection list without running its callback
        void clear_allocated_stuff(e_script_garbage_collection_type type);

        // empties its dynamic arrays and drops its string references and auto-destruct, ready for reconstruction
        void reset();

    private:
        bool       recursive_massacre_threads(const vm_thread* caller, const vm_thread* root);
        vm_thread* delete_thread(vm_thread* thread);

        void release_string_members();
    };

    // the parent object's instance lock, then the instance's thread lock
    class script_instance_lock_scope {

        script_instance* m_instance;

    public:
        explicit script_instance_lock_scope(script_instance* instance);
       ~script_instance_lock_scope();

        script_instance_lock_scope           (const script_instance_lock_scope&) = delete;
        script_instance_lock_scope &operator=(const script_instance_lock_scope&) = delete;
    };

    ASSERT_SIZEOF  (script_instance_callback_node, 0x08);

    ASSERT_SIZEOF  (script_instance_allocated_stuff_record, 0xB4);

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
    ASSERT_OFFSETOF(script_instance, suspend_count,           0x34);
    ASSERT_OFFSETOF(script_instance, client_space,            0x38);
    ASSERT_OFFSETOF(script_instance, thread_lock,             0x40);
    ASSERT_OFFSETOF(script_instance, allocated_stuff_lock,    0x50);
    ASSERT_OFFSETOF(script_instance, list,                    0x60);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_previous, 0x64);
    ASSERT_OFFSETOF(script_instance, vm_simple_list_next,     0x68);
}}} // treyarch::chuck::vm
