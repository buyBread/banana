#include <new>

#include "retail.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/shared/memory/heap.hh"

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

// sub_A1BC60
bool script_manager::load(const string_hash &filename, u32 load_flags, void* user_data, string_hash key_prefix) {
    engine_lock_scope scope(&exec_set_lock);

    script_executable_entry key;
    key.filename   = filename;
    key.key_prefix = key_prefix;

    auto* node = exec_set->find(&key);

    if (node != exec_set->end()) {
        ++node->value->ref_cnt;

        return false;
    }

    void* memory = references::executable_entry_pool.get().allocate();

    script_executable_entry* entry = memory ?
        new (memory) script_executable_entry : nullptr;

    entry->filename   = filename;
    entry->key_prefix = key_prefix;

    i32 resource_size = 0;

    entry->exec = get_script_executable_resource_callback(&filename, &resource_size);

    // retail formats "the script executable '%s' isn't packed" for a stripped print (nullsub_1)
    if (!entry->exec)
        filename.to_string();

    entry->ref_cnt   = 1;
    entry->user_data = user_data;

    run_notification_callback(script_manager_callback_reason_about_to_load, entry->exec, entry);

    entry->exec->post_un_mash_fixup();
    entry->exec->flags = (e_script_executable_flags)(entry->exec->flags | script_executable_flag_un_mashed);

    if (!game_var_container || !shared_var_container)
        entry->exec->flags = (e_script_executable_flags)(entry->exec->flags | script_executable_flag_suspended_for_script_vars);

    if (load_flags & script_manager_load_flag_master)
        master_script = entry->exec;

    entry->exec->link();

    execs_pending_first_run->push_back(entry->exec);
    exec_set->insert(entry);

    entry->exec->flags = (e_script_executable_flags)(entry->exec->flags | script_executable_flag_published);

    run_notification_callback(script_manager_callback_reason_just_loaded, entry->exec, entry);

    return true;
}

// sub_A1C2D0
void script_manager::un_load(const string_hash &filename, bool call_all_destructors, string_hash key_prefix) {
    engine_lock_scope scope(&exec_set_lock);

    script_executable_entry key;
    key.filename   = filename;
    key.key_prefix = key_prefix;

    auto* node = exec_set->find(&key);

    if (node == exec_set->end())
        return;

    script_executable_entry* entry = node->value;

    if (--entry->ref_cnt)
        return;

    if (entry->exec == master_script)
        master_script = nullptr;

    for (auto* pending = execs_pending_first_run->begin(); pending != execs_pending_first_run->end(); pending = pending->next) {
        if (pending->value != entry->exec)
            continue;

        // an executable still waiting on its first run is unloaded without destructors
        call_all_destructors = false;

        execs_pending_first_run->erase(pending);

        break;
    }

    entry->exec->un_load(call_all_destructors);

    script_executable* exec = entry->exec;

    if (exec->flags & script_executable_flag_from_mash)
        exec->flags = (e_script_executable_flags)(exec->flags & ~script_executable_flag_first_run_called);
    else if (exec) {
        retail::sub_A1C250(exec); // script_executable::~script_executable
        memory::heap::free(exec);
    }

    exec_set->erase(node);

    references::executable_entry_pool.get().release(entry);
}

// sub_A1C430
void script_manager::clear() {
    engine_lock_scope scope(&exec_set_lock);

    time_inc      = 0.0f;
    master_script = nullptr;

    while (exec_set->size()) {
        auto* node = exec_set->begin();

        script_executable_entry* entry = node->value;
        script_executable*       exec  = entry->exec;

        exec->un_load(false);

        if (!(exec->flags & script_executable_flag_from_mash)) {
            retail::sub_A1C250(exec); // script_executable::~script_executable
            memory::heap::free(exec);
        }

        exec_set->erase(node);

        references::executable_entry_pool.get().release(entry);
    }

    execs_pending_first_run->clear();
}

// sub_A1A170
bool script_manager::is_loadable(const char* filename) {
    string_hash filename_hash;
    filename_hash.initialize(mash::ALLOCATED, filename);

    i32 resource_size;

    return get_script_executable_resource_callback(&filename_hash, &resource_size) != nullptr;
}

// sub_A1B840
bool script_manager::is_loaded(const string_hash &filename, string_hash key_prefix) {
    engine_lock_scope scope(&exec_set_lock);

    script_executable_entry key;
    key.filename   = filename;
    key.key_prefix = key_prefix;

    return exec_set->find(&key) != exec_set->end();
}

// sub_A1B7A0
script_executable* script_manager::find_executable(const string_hash &filename, string_hash key_prefix) {
    engine_lock_scope scope(&exec_set_lock);

    script_executable_entry key;
    key.filename   = filename;
    key.key_prefix = key_prefix;

    auto* node = exec_set->find(&key);

    if (node == exec_set->end())
        return nullptr;

    return node->value->exec;
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

// sub_A1B0E0
void script_manager::unsuspend_execs_for_script_vars() {
    engine_lock_scope scope(&exec_set_lock);

    for (auto* node = exec_set->begin(); node != exec_set->end(); node = node->next()) {
        script_executable* exec = node->value->exec;

        if (!(exec->flags & script_executable_flag_suspended_for_script_vars))
            continue;

        exec->flags = (e_script_executable_flags)(exec->flags & ~script_executable_flag_suspended_for_script_vars);
        exec->link();
    }
}

// sub_A1B8C0
void script_manager::init_game_var() {
    string_hash master;
    i32         resource_size;

    if (!game_var_container) {
        master.initialize(mash::ALLOCATED, "master");

        game_var_container = get_script_var_container_resource_callback(&master, &resource_size, true);
    }

    if (!shared_var_container) {
        master.initialize(mash::ALLOCATED, "master");

        shared_var_container = get_script_var_container_resource_callback(&master, &resource_size, false);
    }

    unsuspend_execs_for_script_vars();
}

// sub_A1BF30
void script_manager::destroy_game_var() {
    if (game_var_container) {
        if (!(game_var_container->flags & script_var_container_flag_from_mash)) {
            retail::sub_A1BC00(game_var_container); // script_var_container::~script_var_container
            memory::heap::free(game_var_container);
        }

        game_var_container = nullptr;
    }

    if (shared_var_container) {
        if (!(shared_var_container->flags & script_var_container_flag_from_mash)) {
            retail::sub_A1BC00(shared_var_container); // script_var_container::~script_var_container
            memory::heap::free(shared_var_container);
        }

        shared_var_container = nullptr;
    }
}

// sub_A1A1B0
u8* script_manager::get_game_var_address(const mash::string &game_var_name, bool* is_game_var) const {
    if (is_game_var)
        *is_game_var = false;

    u8* address = game_var_container->get_address(game_var_name.c_str());

    if (!address)
        return shared_var_container->get_address(game_var_name.c_str());

    if (is_game_var)
        *is_game_var = true;

    return address;
}

// sub_A1A200
u8* script_manager::get_game_var_addr(const char* game_var_name) const {
    return game_var_container->get_address(game_var_name);
}

// sub_A1A220
u8* script_manager::get_shared_var_addr(const char* shared_var_name) const {
    return shared_var_container->get_address(shared_var_name);
}

// sub_A1A210
u8* script_manager::get_game_var_address(i32 offset) const {
    return game_var_container->script_var_block.buffer + offset;
}

// sub_A1A230
u8* script_manager::get_shared_var_address(i32 offset) const {
    return shared_var_container->script_var_block.buffer + offset;
}
