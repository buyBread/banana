#include "retail.hh"
#include "treyarch/chuck/vm/script_manager.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A1A2C0
void script_manager::register_callbacks(notification_callback_t                      notification,
                                        get_script_executable_resource_callback_t    get_script_executable_resource,
                                        get_script_var_container_resource_callback_t get_script_var_container_resource,
                                        unk_predicate_callback_t                     unk_predicate_0,
                                        unk_predicate_callback_t                     unk_predicate_1,
                                        unk_predicate_callback_t                     unk_predicate_2,
                                        unk_predicate_callback_t                     unk_predicate_3,
                                        get_platform_callback_t                      get_platform) {

    notification_callback                      = notification;
    get_script_executable_resource_callback    = get_script_executable_resource;
    get_script_var_container_resource_callback = get_script_var_container_resource;
    unk_predicate_callbacks[0]                 = unk_predicate_0;
    unk_predicate_callbacks[1]                 = unk_predicate_1;
    unk_predicate_callbacks[2]                 = unk_predicate_2;
    unk_predicate_callbacks[3]                 = unk_predicate_3;
    get_platform_callback                      = get_platform;
}

// sub_A1A280
void script_manager::run_notification_callback(e_script_manager_callback_reason reason, script_executable* se, void* user_data) {
    engine_lock_scope scope(&notification_lock);

    notification_callback(reason, se, user_data);
}

// sub_A1A120
script_instance_garbage_collection_callback_t script_manager::get_garbage_collection_callback(e_script_garbage_collection_type type) const {
    return garbage_collection_callbacks[type];
}

// inlined @ sub_A1AE50, sub_A1B960
void script_manager::first_run_pending_execs(f32 requested_time_inc, bool ignore_suspended) {
    if (execs_pending_first_run->empty())
        return;

    for (auto* node = execs_pending_first_run->begin(); node != execs_pending_first_run->end(); node = node->next)
        node->value->first_run(requested_time_inc, ignore_suspended);

    execs_pending_first_run->clear();
}

// sub_A1AE50
void script_manager::run(f32 requested_time_inc, bool ignore_suspended) {
    u32 &guard = references::run_name_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        references::run_name.get().initialize(mash::ALLOCATED, "script_manager");
        retail::sub_ADE5FD(retail::sub_B77CD0); // the game's CRT atexit
    }

    if (!game_var_container || !shared_var_container)
        return;

    engine_lock_scope scope(&exec_set_lock);

    time_inc = requested_time_inc;

    first_run_pending_execs(requested_time_inc, ignore_suspended);

    for (auto* node = exec_set->begin(); node != exec_set->end(); node = node->next()) {
        script_executable* exec      = node->value->exec;
        void*              user_data = node->value->user_data;

        if (!(exec->flags & script_executable_flag_needs_run))
            continue;

        run_notification_callback(script_manager_callback_reason_about_to_run, exec, user_data);
        exec->run(requested_time_inc, ignore_suspended);
        run_notification_callback(script_manager_callback_reason_just_ran, exec, user_data);
    }
}

// sub_A1B030
vm_thread* script_manager::find_thread(u32 thread_id) {
    engine_lock_scope scope(&exec_set_lock);

    for (auto* node = exec_set->begin(); node != exec_set->end(); node = node->next()) {
        vm_thread* thread = node->value->exec->find_thread(thread_id);

        if (thread)
            return thread;
    }

    return nullptr;
}

// sub_A1B960
bool script_manager::run_single_exec(const string_hash &filename, string_hash key_prefix, f32 requested_time_inc, bool ignore_suspended) {
    if (!game_var_container || !shared_var_container)
        return false;

    engine_lock_scope scope(&exec_set_lock);

    time_inc = requested_time_inc;

    first_run_pending_execs(requested_time_inc, ignore_suspended);

    script_executable_entry key;
    key.filename   = filename;
    key.key_prefix = key_prefix;

    auto* node = exec_set->find(&key);

    if (node == exec_set->end())
        return false;

    script_executable* exec = node->value->exec;

    run_notification_callback(script_manager_callback_reason_about_to_run, exec, node->value->user_data);
    exec->run(requested_time_inc, ignore_suspended);
    run_notification_callback(script_manager_callback_reason_just_ran, exec, node->value->user_data);

    return true;
}
