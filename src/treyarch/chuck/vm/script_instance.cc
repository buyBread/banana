#include <cstring>
#include <new>

#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

script_instance_lock_scope::script_instance_lock_scope(script_instance* instance) : m_instance(instance) {
    m_instance->parent->instance_lock->lock_ref();
    m_instance->thread_lock.acquire();
}

// sub_A1DD20
script_instance_lock_scope::~script_instance_lock_scope() {
    m_instance->thread_lock.release();
    m_instance->parent->instance_lock->unlock_ref();
}

// sub_A1C600
void* script_instance::operator new(std::size_t) noexcept {
    return references::instance_pool.get().allocate();
}

// sub_A1C6A0
void script_instance::operator delete(void* allocation) noexcept {
    references::instance_pool.get().release(allocation);
}

// sub_A1DBA0
void* script_instance_callback_node::operator new(std::size_t) noexcept {
    return references::callback_node_pool.get().allocate();
}

void script_instance_callback_node::operator delete(void* allocation) noexcept {
    references::callback_node_pool.get().release(allocation);
}

// sub_A1DA20
void* garbage_collection_element::operator new(std::size_t) noexcept {
    return references::garbage_collection_element_pool.get().allocate();
}

void garbage_collection_element::operator delete(void* allocation) noexcept {
    references::garbage_collection_element_pool.get().release(allocation);
}

// inlined @ sub_A1F4F0
script_instance_allocated_stuff_record::script_instance_allocated_stuff_record() {
    for (i32 index = 0; index < script_garbage_collection_total_types; ++index) {
        lists[index].head = nullptr;
        lists[index].last = nullptr;
        lists[index].size = 0;
    }
}

// sub_A1DAC0
void* script_instance_allocated_stuff_record::operator new(std::size_t) noexcept {
    return references::allocated_stuff_pool.get().allocate();
}

// sub_A1DB60
void script_instance_allocated_stuff_record::operator delete(void* allocation) noexcept {
    references::allocated_stuff_pool.get().release(allocation);
}

// sub_A1E420
script_instance::script_instance(const char* requested_name, i32 data_size, u32 requested_flags) {
    flags = (e_script_instance_flags)(requested_flags | 0x40400000);

    name.initialize(mash::ALLOCATED, requested_name);

    threads.head   = nullptr;
    threads.last   = nullptr;
    threads.size   = 0;
    parent         = nullptr;
    callback       = nullptr;
    callback_nodes = nullptr;

    allocated_stuff = nullptr;

    suspend_count           = 0;
    client_space            = nullptr;
    list                    = nullptr;
    vm_simple_list_previous = nullptr;
    vm_simple_list_next     = nullptr;

    thread_lock.owner          = 0;
    thread_lock.state          = 0;
    thread_lock.depth          = 0;
    allocated_stuff_lock.owner = 0;
    allocated_stuff_lock.state = 0;
    allocated_stuff_lock.depth = 0;

    data.setup(data_size);

    if (references::script_instance_created_callback.read())
        references::script_instance_created_callback.read()(this);
}

// sub_A1F140
script_instance::~script_instance() {
    run_callbacks(script_instance_callback_reason_instance_about_to_die, nullptr);

    vm_thread* thread = threads.head;

    if (thread) {
        do {
            if (threads.head)
                threads.erase(threads.head);

            delete thread;

            thread = threads.head;
        } while (threads.head);
    }

    // the lists are left as they are; do_garbage_collection empties them first
    delete allocated_stuff;

    release_string_members();

    if (references::script_instance_destroyed_callback.read())
        references::script_instance_destroyed_callback.read()(this);
}

// sub_A1E2B0
void script_instance::release_string_members() {
    script_object* object = parent;

    if (!object)
        return;

    do {
        u32 count = object->reference_descriptors.size;

        for (u32 index = 0; index < count; ++index) {
            const vm_reference_descriptor &descriptor = object->reference_descriptors.data[index];

            if (!descriptor.kind)
                vm_dynamic_array_manager::inst()->remove_string_reference(*(void**)(data.buffer + descriptor.offset));
        }

        object = object->parent_object;
    } while (object && !(object->flags & script_object_flag_global_object));
}

// sub_A1DD80
void script_instance::set_stack_size(e_script_instance_stack_size stack_size) {
    const u32 stack_flags = script_instance_flag_small_stack | script_instance_flag_large_stack;

    switch (stack_size) {
        case script_instance_stack_size_small:
            flags = (e_script_instance_flags)((flags & ~stack_flags) | script_instance_flag_small_stack);
            break;

        case script_instance_stack_size_normal:
            flags = (e_script_instance_flags)(flags & ~stack_flags);
            break;

        case script_instance_stack_size_large:
            flags = (e_script_instance_flags)((flags & ~stack_flags) | script_instance_flag_large_stack);
            break;
    }
}

// sub_A1E980
void script_instance::run(bool ignore_suspended) {
    script_instance_lock_scope scope(this);

    script_executable* exec = parent->parent;

    if ((exec->flags & script_executable_flag_suspended_for_script_vars) || exec->suspend_count > 0)
        return;

    if (suspend_count > 0 && !ignore_suspended)
        return;

    flags = (e_script_instance_flags)(flags | script_instance_flag_run_called);

    vm_thread* thread = threads.head;

    if (!thread)
        return;

    do {
        // true once the thread has finished
        if (thread->run())
            thread = delete_thread(thread);
        else
            thread = thread->vm_simple_list_next;
    } while (exec->suspend_count <= 0 && thread);
}

// sub_A1EAB0
bool script_instance::run_single_thread(vm_thread* t, bool ignore_suspended, string_hash key_prefix) {
    script_instance_lock_scope scope(this);

    script_executable* exec = parent->parent;

    if ((exec->flags & script_executable_flag_suspended_for_script_vars) || exec->suspend_count > 0)
        return false;

    if (suspend_count > 0 && !ignore_suspended)
        return false;

    flags = (e_script_instance_flags)(flags | script_instance_flag_run_called);

    if (!t->run())
        return false;

    for (vm_thread* thread = threads.head; thread; thread = thread->vm_simple_list_next) {
        if (thread == t) {
            delete_thread(thread);

            break;
        }
    }

    return true;
}

// sub_A1EC00
bool script_instance::run_single_thread_until_finished(vm_thread*  t,
                                                       bool        ignore_suspended,
                                                       string_hash key_prefix) {

    {
        script_instance_lock_scope scope(this);

        script_executable* exec = parent->parent;

        if ((exec->flags & script_executable_flag_suspended_for_script_vars) || exec->suspend_count > 0)
            return false;

        if (suspend_count > 0 && !ignore_suspended)
            return false;
    }

    // the locks are dropped between passes
    while (!run_single_thread(t, ignore_suspended, key_prefix));

    return true;
}

// sub_A1ECF0
bool script_instance::run_single_thread_with_return(vm_thread*  t,
                                                    bool        ignore_suspended,
                                                    void*       ret,
                                                    u32         return_value_byte_size,
                                                    string_hash key_prefix) {

    script_instance_lock_scope scope(this);

    script_executable* exec = parent->parent;

    if ((exec->flags & script_executable_flag_suspended_for_script_vars) || exec->suspend_count > 0)
        return false;

    if (suspend_count > 0 && !ignore_suspended)
        return false;

    flags = (e_script_instance_flags)(flags | script_instance_flag_run_called);

    if (!t->run())
        return false;

    // the return value is what the finished thread left on top of its stack
    if (ret)
        std::memcpy(ret, t->dstack.sp - return_value_byte_size, return_value_byte_size);

    for (vm_thread* thread = threads.head; thread; thread = thread->vm_simple_list_next) {
        if (thread == t) {
            delete_thread(thread);

            break;
        }
    }

    return true;
}

// sub_A1E670
vm_thread* script_instance::delete_thread(vm_thread* thread) {
    script_instance_lock_scope scope(this);

    // children forget a dying creator
    for (vm_thread* other = threads.head; other; other = other->vm_simple_list_next) {
        if (other != thread && other->creator == thread)
            other->creator = nullptr;
    }

    vm_thread* next = threads.erase(thread);

    delete thread;

    return next;
}

// sub_A1E7A0
void script_instance::kill_thread(const script_function* ex, const vm_thread* ignore_thread) {
    script_instance_lock_scope scope(this);

    vm_thread* thread = threads.head;

    while (thread) {
        bool matches = thread->inst == this && thread != ignore_thread && thread->ex->name == ex->name;

        if (matches)
            thread = delete_thread(thread);
        else
            thread = thread->vm_simple_list_next;
    }
}

// sub_A1E8B0
void script_instance::kill_thread(vm_thread* thread_to_kill) {
    script_instance_lock_scope scope(this);

    for (vm_thread* thread = threads.head; thread; thread = thread->vm_simple_list_next) {
        if (thread == thread_to_kill) {
            delete_thread(thread);

            return;
        }
    }
}

// sub_A1E590
bool script_instance::thread_exists(u32 thread_id) {
    script_instance_lock_scope scope(this);

    for (vm_thread* thread = threads.head; thread; thread = thread->vm_simple_list_next) {
        if (thread->thread_id == thread_id)
            return true;
    }

    return false;
}

// sub_A1E640
bool script_instance::thread_exists(vm_thread* thread, u32 thread_id) {
    return thread_exists(thread_id) && thread->thread_id == thread_id;
}

// sub_A1F270
bool script_instance::massacre_threads_by_function(const script_function* ex, const vm_thread* ignore_thread) {
    script_instance_lock_scope scope(this);

    bool found = false;

    vm_thread* thread = threads.head;

    // without a function every thread but ignore_thread dies
    if (!ex) {
        while (thread) {
            if (thread == ignore_thread) {
                thread = thread->vm_simple_list_next;
                found  = true;
            } else
                thread = delete_thread(thread);
        }

        return found;
    }

    while (thread) {
        if (!(thread->ex->name == ex->name)) {
            thread = thread->vm_simple_list_next;

            continue;
        }

        vm_thread* creator = thread->creator;

        thread->creator = nullptr;

        if (recursive_massacre_threads(ignore_thread, thread))
            found = true;

        thread->creator = creator;

        if (thread == ignore_thread)
            thread = thread->vm_simple_list_next;
        else
            thread = delete_thread(thread);
    }

    return found;
}

// sub_A1F400
bool script_instance::massacre_threads_by_thread(vm_thread* thread_to_massacre, const vm_thread* ignore_thread) {
    script_instance_lock_scope scope(this);

    vm_thread* thread = threads.head;

    while (thread && thread != thread_to_massacre)
        thread = thread->vm_simple_list_next;

    if (!thread)
        return false;

    bool found = false;

    vm_thread* creator = thread->creator;

    thread->creator = nullptr;

    if (recursive_massacre_threads(ignore_thread, thread))
        found = true;

    thread->creator = creator;

    if (thread != ignore_thread)
        delete_thread(thread);

    return found;
}

// sub_A1E140
void script_instance::enable_auto_destruct() {
    flags = (e_script_instance_flags)(flags | script_instance_flag_auto_destruct);

    if (!threads.head) {
        parent->flags         = (e_script_object_flags)    (parent->flags         | script_object_flag_needs_run);
        parent->parent->flags = (e_script_executable_flags)(parent->parent->flags | script_executable_flag_needs_run);
    }
}

// sub_A1DD60
void script_instance::suspend() {
    ++suspend_count;
}

// sub_A1DD70
void script_instance::unsuspend() {
    if (--suspend_count < 0)
        suspend_count = 0;
}

// sub_A1EF10
bool script_instance::recursive_massacre_threads(const vm_thread* caller, const vm_thread* root) {
    script_instance_lock_scope scope(this);

    bool found = false;

    vm_thread* thread = threads.head;

    while (thread) {
        if (thread->creator != root) {
            thread = thread->vm_simple_list_next;

            continue;
        }

        if (recursive_massacre_threads(caller, thread))
            found = true;

        if (thread == caller) {
            found  = true;
            thread = thread->vm_simple_list_next;
        } else
            thread = delete_thread(thread);
    }

    return found;
}

// sub_A1DE70
void script_instance::run_callbacks(e_script_instance_callback_reason reason, vm_thread* vmt) {
    script_instance_lock_scope scope(this);

    flags = (e_script_instance_flags)(flags | script_instance_flag_running_callbacks);

    for (script_instance_callback_node* node = callback_nodes; node; node = node->next)
        callback(reason, this, vmt, node->user_data);

    if (reason == script_instance_callback_reason_instance_about_to_die && callback_nodes) {
        do {
            script_instance_callback_node* node = callback_nodes;

            callback_nodes = node->next;

            delete node;
        } while (callback_nodes);
    }

    flags = (e_script_instance_flags)(flags & ~script_instance_flag_running_callbacks);
}

// sub_A1DF80
void script_instance::set_script_instance_callbacks(script_instance_created_callback_t   created_callback,
                                                    script_instance_destroyed_callback_t destroyed_callback) {

    references::script_instance_created_callback  .write(created_callback);
    references::script_instance_destroyed_callback.write(destroyed_callback);
}

// sub_A1DDC0
void script_instance::register_callback(script_instance_callback_t requested_callback, void* user_data) {
    script_instance_lock_scope scope(this);

    if (requested_callback)
        callback = requested_callback;

    script_instance_callback_node* node = new script_instance_callback_node { user_data, nullptr };

    node->next     = callback_nodes;
    callback_nodes = node;
}

// sub_A1E160
void script_instance::unregister_callback(void* user_data) {
    script_instance_lock_scope scope(this);

    script_instance_callback_node* previous = nullptr;
    script_instance_callback_node* node     = callback_nodes;

    while (node) {
        if (node->user_data != user_data) {
            previous = node;
            node     = node->next;

            continue;
        }

        if (previous) {
            previous->next = node->next;

            delete node;

            node = previous->next;
        } else {
            callback_nodes = node->next;

            delete node;

            node = callback_nodes;
        }
    }
}

// sub_A1DFA0
vm_thread* script_instance::add_thread(const script_function* ex, void* user_data, u32 stack_size) {
    script_instance_lock_scope scope(this);

    if (!stack_size) {
        if (flags & script_instance_flag_small_stack)
            stack_size = 128;
        else if (flags & script_instance_flag_large_stack)
            stack_size = 512;
        else if (ex->flags & script_function_flag_small_stack_recommended)
            stack_size = 128;
        else
            stack_size = (ex->flags & script_function_flag_large_stack_recommended) ? 512 : 284;
    }

    vm_thread* thread = new vm_thread(this, ex, user_data, stack_size);

    thread->list                    = &threads;
    thread->vm_simple_list_previous = threads.last;
    thread->vm_simple_list_next     = nullptr;

    if (threads.last)
        threads.last->vm_simple_list_next = thread;

    bool was_empty = !threads.head;

    threads.last = thread;

    if (was_empty)
        threads.head = thread;

    ++threads.size;

    parent->flags         = (e_script_object_flags)    (parent->flags | script_object_flag_needs_run);
    parent->parent->flags = (e_script_executable_flags)(parent->parent->flags | script_executable_flag_needs_run);

    return thread;
}

// sub_A1E0F0
vm_thread* script_instance::add_thread(const script_function* ex,
                                       const void*            arguments,
                                             i32              argument_size,
                                             void*            user_data,
                                             u32              stack_size) {

    vm_thread* thread = add_thread(ex, user_data, stack_size);

    if (!thread)
        return nullptr;

    if (argument_size > 0)
        thread->dstack.push(arguments, argument_size);

    ex->add_thread_references(thread, true, true);

    return thread;
}

// sub_A1F4F0
void script_instance::add_allocated_stuff(e_script_garbage_collection_type type, u32 stuff) {
    engine_lock_scope scope(&allocated_stuff_lock);

    garbage_collection_element* element = new garbage_collection_element { stuff, nullptr, nullptr, nullptr };

    if (!allocated_stuff)
        allocated_stuff = new script_instance_allocated_stuff_record;

    // pushed at the head
    stuff_to_delete_list_t &stuff_list = allocated_stuff->lists[type];

    element->list                    = &stuff_list;
    element->vm_simple_list_previous = nullptr;
    element->vm_simple_list_next     = stuff_list.head;

    if (stuff_list.head)
        stuff_list.head->vm_simple_list_previous = element;

    bool was_empty = !stuff_list.last;

    stuff_list.head = element;

    if (was_empty)
        stuff_list.last = element;

    ++stuff_list.size;
}

// sub_A1F000
void script_instance::do_garbage_collection() {
    engine_lock_scope scope(&allocated_stuff_lock);

    if (!allocated_stuff)
        return;

    for (i32 type = 0; type < script_garbage_collection_total_types; ++type) {
        stuff_to_delete_list_t &stuff_list = allocated_stuff->lists[type];

        if (!stuff_list.head)
            continue;

        script_manager::inst()->get_garbage_collection_callback((e_script_garbage_collection_type)type)(this, stuff_list);

        while (stuff_list.head) {
            garbage_collection_element* element = stuff_list.head;

            if (element) {
                stuff_list.erase(element);

                delete element;
            }
        }
    }

    delete allocated_stuff;

    allocated_stuff = nullptr;
}

// sub_A1E4D0
void script_instance::remove_allocated_stuff(e_script_garbage_collection_type type, u32 stuff) {
    engine_lock_scope scope(&allocated_stuff_lock);

    if (!allocated_stuff)
        return;

    stuff_to_delete_list_t &stuff_list = allocated_stuff->lists[type];

    garbage_collection_element* element = stuff_list.head;

    while (element) {
        if (element->element != stuff) {
            element = element->vm_simple_list_next;

            continue;
        }

        garbage_collection_element* removed = element;

        element = stuff_list.erase(element);

        delete removed;
    }
}

// sub_A1EE60
void script_instance::clear_allocated_stuff(e_script_garbage_collection_type type) {
    engine_lock_scope scope(&allocated_stuff_lock);

    if (!allocated_stuff)
        return;

    stuff_to_delete_list_t &stuff_list = allocated_stuff->lists[type];

    while (stuff_list.head) {
        garbage_collection_element* element = stuff_list.head;

        if (element) {
            stuff_list.erase(element);

            delete element;
        }
    }
}

// sub_A1F5D0
void script_instance::reset() {
    {
        engine_lock_scope scope(&allocated_stuff_lock);

        if (allocated_stuff) {
            stuff_to_delete_list_t &arrays = allocated_stuff->lists[script_garbage_collection_dynamic_array];

            for (garbage_collection_element* element = arrays.head; element; element = element->vm_simple_list_next)
                ((chuck_dynamic_array_t*)element->element)->clear();
        }
    }

    release_string_members();

    flags = (e_script_instance_flags)(flags & ~script_instance_flag_auto_destruct);
}
