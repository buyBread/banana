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

            if (thread) {
                thread->~vm_thread();

                references::thread_pool.get().release(thread);
            }

            thread = threads.head;
        } while (threads.head);
    }

    // the lists are left as they are; do_garbage_collection empties them first
    if (allocated_stuff)
        references::allocated_stuff_pool.get().release(allocated_stuff);

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
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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

// sub_A1E670
vm_thread* script_instance::delete_thread(vm_thread* thread) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

    // children forget a dying creator
    for (vm_thread* other = threads.head; other; other = other->vm_simple_list_next) {
        if (other != thread && other->creator == thread)
            other->creator = nullptr;
    }

    vm_thread* next = threads.erase(thread);

    if (thread) {
        thread->~vm_thread();

        references::thread_pool.get().release(thread);
    }

    return next;
}

// sub_A1E7A0
void script_instance::kill_thread(const script_function* ex, const vm_thread* ignore_thread) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

    for (vm_thread* thread = threads.head; thread; thread = thread->vm_simple_list_next) {
        if (thread == thread_to_kill) {
            delete_thread(thread);

            return;
        }
    }
}

// sub_A1F270
bool script_instance::massacre_threads_by_function(const script_function* ex, const vm_thread* ignore_thread) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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

// sub_A1EF10
bool script_instance::recursive_massacre_threads(const vm_thread* caller, const vm_thread* root) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

    flags = (e_script_instance_flags)(flags | script_instance_flag_running_callbacks);

    for (script_instance_callback_node* node = callback_nodes; node; node = node->next)
        callback(reason, this, vmt, node->user_data);

    if (reason == script_instance_callback_reason_instance_about_to_die && callback_nodes) {
        do {
            script_instance_callback_node* node = callback_nodes;

            callback_nodes = node->next;

            if (node)
                references::callback_node_pool.get().release(node);
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
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

    if (requested_callback)
        callback = requested_callback;

    void* memory = references::callback_node_pool.get().allocate();

    script_instance_callback_node* node = memory ?
        new (memory) script_instance_callback_node { user_data, nullptr } : nullptr;

    node->next     = callback_nodes;
    callback_nodes = node;
}

// sub_A1E160
void script_instance::unregister_callback(void* user_data) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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

            references::callback_node_pool.get().release(node);

            node = previous->next;
        } else {
            callback_nodes = node->next;

            references::callback_node_pool.get().release(node);

            node = callback_nodes;
        }
    }
}

// sub_A1DFA0
vm_thread* script_instance::add_thread(const script_function* ex, void* user_data, u32 stack_size) {
    ref_lock_scope    instance_scope(parent->instance_lock);
    engine_lock_scope thread_scope(&thread_lock);

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

    void* memory = references::thread_pool.get().allocate();

    vm_thread* thread = memory ?
        new (memory) vm_thread(this, ex, user_data, stack_size) : nullptr;

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

    void* memory = references::garbage_collection_element_pool.get().allocate();

    garbage_collection_element* element = memory ?
        new (memory) garbage_collection_element { stuff, nullptr, nullptr, nullptr } : nullptr;

    if (!allocated_stuff) {
        auto* lists = (vm_simple_list<garbage_collection_element*>*)references::allocated_stuff_pool.get().allocate();

        if (lists) {
            for (i32 index = 0; index < script_garbage_collection_total_types; ++index)
                new (&lists[index]) vm_simple_list<garbage_collection_element*> { nullptr, nullptr, 0 };
        }

        allocated_stuff = lists;
    }

    // pushed at the head
    vm_simple_list<garbage_collection_element*> &stuff_list = allocated_stuff[type];

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
        if (!allocated_stuff[type].head)
            continue;

        script_manager::inst()->get_garbage_collection_callback((e_script_garbage_collection_type)type)(this, allocated_stuff[type]);

        while (allocated_stuff[type].head) {
            garbage_collection_element* element = allocated_stuff[type].head;

            if (element) {
                allocated_stuff[type].erase(element);

                references::garbage_collection_element_pool.get().release(element);
            }
        }
    }

    if (allocated_stuff)
        references::allocated_stuff_pool.get().release(allocated_stuff);

    allocated_stuff = nullptr;
}

// sub_A1E4D0
void script_instance::remove_allocated_stuff(e_script_garbage_collection_type type, u32 stuff) {
    engine_lock_scope scope(&allocated_stuff_lock);

    if (!allocated_stuff)
        return;

    vm_simple_list<garbage_collection_element*> &stuff_list = allocated_stuff[type];

    garbage_collection_element* element = stuff_list.head;

    while (element) {
        if (element->element != stuff) {
            element = element->vm_simple_list_next;

            continue;
        }

        garbage_collection_element* removed = element;

        element = stuff_list.erase(element);

        references::garbage_collection_element_pool.get().release(removed);
    }
}
