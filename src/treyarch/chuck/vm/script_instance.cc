#include <new>

#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

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

    thread->thread_list             = &threads;
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
                                       i32                    argument_size,
                                       void*                  user_data,
                                       u32                    stack_size) {

    vm_thread* thread = add_thread(ex, user_data, stack_size);

    if (!thread)
        return nullptr;

    if (argument_size > 0)
        thread->dstack.push(arguments, argument_size);

    ex->add_thread_references(thread, true, true);

    return thread;
}

// sub_A1E4D0
void script_instance::remove_allocated_stuff(e_script_garbage_collection_type type, u32 stuff) {
    engine_lock_scope scope(&allocated_stuff_lock);

    if (!allocated_stuff)
        return;

    vm_simple_list<garbage_collection_element*> &list = allocated_stuff[type];

    garbage_collection_element* element = list.head;

    while (element) {
        if (element->element != stuff) {
            element = element->vm_simple_list_next;

            continue;
        }

        garbage_collection_element* removed = element;

        element = list.erase(element);

        references::garbage_collection_element_pool.get().release(removed);
    }
}
