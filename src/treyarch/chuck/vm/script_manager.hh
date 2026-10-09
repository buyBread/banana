#pragma once

#include <cassert>

#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_executable_entry.hh"
#include "treyarch/chuck/vm/script_var_container.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/platform.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    enum e_script_manager_callback_reason : i32 {
        script_manager_callback_reason_just_unloaded,
        script_manager_callback_reason_about_to_unload,
        script_manager_callback_reason_just_loaded,
        script_manager_callback_reason_about_to_load,
        script_manager_callback_reason_error_msg,
        script_manager_callback_reason_warning_msg,
        script_manager_callback_reason_about_to_run, // brackets every run of one executable with...
        script_manager_callback_reason_just_ran      // ...this; the game switches resource context here
    };

    // SM3 passes a message buffer third; retail passes the entry's user_data
    using notification_callback_t = void(*)(e_script_manager_callback_reason reason,
                                            script_executable*               se,
                                            void*                            user_data);
    using get_script_executable_resource_callback_t = script_executable*(*)(const string_hash* filename,
                                                                                  i32*         resource_size);
    using get_script_var_container_resource_callback_t = script_var_container*(*)(const string_hash* filename,
                                                                                        i32*         resource_size,
                                                                                        bool         is_game_var_container);
    using get_platform_callback_t = e_platform(*)();

    // four byte-returning slots where SM3 had one;
    // all installed as `return false` (sub_71F870)
    using unk_predicate_callback_t = bool(*)();

    /* created once by game::game (sub_97AB10) and kept for the process.
       nothing runs until both variable containers exist (sub_A1AE50, sub_A1B960).
       lock order: exec_set_lock, then object instance_lock, then instance thread_lock. */
    class script_manager : public singleton_instance<script_manager, 0x011248E0> {

    public:
        u32                                          flags;                            // SM3 position; no retail consumer found
        f32                                          time_inc;
        script_executable_entry_set_t*               exec_set;
        script_executable*                           master_script;
        script_var_container*                        game_var_container;
        script_var_container*                        shared_var_container;
        dinkumware::list<script_executable*>*        execs_pending_first_run;
        void*                                        garbage_collection_callbacks[15]; // SM3 had 14 types
        notification_callback_t                      notification_callback;
        get_script_executable_resource_callback_t    get_script_executable_resource_callback;
        get_script_var_container_resource_callback_t get_script_var_container_resource_callback;
        unk_predicate_callback_t                     unk_predicate_callbacks[4];      // SM3's using_chuck_old_fashioned_callback is presumably one of these
        get_platform_callback_t                      get_platform_callback;
        engine_recursive_lock                        exec_set_lock;                   // held across load, run, and unload
        engine_recursive_lock                        notification_lock;

        void register_callbacks(notification_callback_t                      notification,
                                get_script_executable_resource_callback_t    get_script_executable_resource,
                                get_script_var_container_resource_callback_t get_script_var_container_resource,
                                unk_predicate_callback_t                     unk_predicate_0,
                                unk_predicate_callback_t                     unk_predicate_1,
                                unk_predicate_callback_t                     unk_predicate_2,
                                unk_predicate_callback_t                     unk_predicate_3,
                                get_platform_callback_t                      get_platform);
    };

    ASSERT_SIZEOF  (script_manager,                                             0x98);
    ASSERT_OFFSETOF(script_manager, flags,                                      0x00);
    ASSERT_OFFSETOF(script_manager, time_inc,                                   0x04);
    ASSERT_OFFSETOF(script_manager, exec_set,                                   0x08);
    ASSERT_OFFSETOF(script_manager, master_script,                              0x0C);
    ASSERT_OFFSETOF(script_manager, game_var_container,                         0x10);
    ASSERT_OFFSETOF(script_manager, shared_var_container,                       0x14);
    ASSERT_OFFSETOF(script_manager, execs_pending_first_run,                    0x18);
    ASSERT_OFFSETOF(script_manager, garbage_collection_callbacks,               0x1C);
    ASSERT_OFFSETOF(script_manager, notification_callback,                      0x58);
    ASSERT_OFFSETOF(script_manager, get_script_executable_resource_callback,    0x5C);
    ASSERT_OFFSETOF(script_manager, get_script_var_container_resource_callback, 0x60);
    ASSERT_OFFSETOF(script_manager, unk_predicate_callbacks,                    0x64);
    ASSERT_OFFSETOF(script_manager, get_platform_callback,                      0x74);
    ASSERT_OFFSETOF(script_manager, exec_set_lock,                              0x78);
    ASSERT_OFFSETOF(script_manager, notification_lock,                          0x88);
}}} // treyarch::chuck::vm
