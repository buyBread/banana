#include <new>

#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A1CF70
void script_object::add(script_instance* inst) {
    ref_lock_scope scope(instance_lock);

    inst->parent                  = this;
    inst->list                    = &instances;
    inst->vm_simple_list_previous = instances.last;
    inst->vm_simple_list_next     = nullptr;

    if (instances.last)
        instances.last->vm_simple_list_next = inst;

    bool was_empty = !instances.head;

    instances.last = inst;

    if (was_empty)
        instances.head = inst;

    ++instances.size;
}

// sub_A1D220
script_instance* script_object::add_instance(const char* inst_name, e_script_instance_stack_size stack_size) {
    void* memory = references::instance_pool.get().allocate();

    script_instance* inst = memory ?
        new (memory) script_instance(inst_name, data_blocksize, 0) : nullptr;

    add(inst);

    ref_lock_scope    instance_scope(inst->parent->instance_lock);
    engine_lock_scope thread_scope(&inst->thread_lock);

    inst->set_stack_size(stack_size);

    construct_instance(inst, nullptr, nullptr);

    return inst;
}

// sub_A1D190
script_instance* script_object::add_instance(const mash::string                 &inst_name,
                                             const void*                         constructor_parms_buffer,
                                                   vm_thread**                   constructor_thread,
                                                   e_script_instance_stack_size  stack_size) {

    void* memory = references::instance_pool.get().allocate();

    script_instance* inst = memory ?
        new (memory) script_instance(inst_name.c_str(), data_blocksize, 0) : nullptr;

    inst->set_stack_size(stack_size);

    add(inst);

    construct_instance(inst, constructor_parms_buffer, constructor_thread);

    return inst;
}

// sub_A1CCE0
void script_object::construct_instance(script_instance* inst, const void* constructor_parms_buffer, vm_thread** constructor_thread) {
    inst->parent->instance_lock->lock_ref();
    inst->thread_lock.acquire();

    // without a constructor both locks stay held
    if (constructor_index < 0 || (u32)constructor_index >= funcs.size)
        return;

    const script_function* constructor = funcs[constructor_index];

    vm_thread* thread = inst->add_thread(constructor, nullptr, 0);

    thread->dstack.push(&inst, sizeof(inst));

    i32 parms_size = constructor->parms_stacksize;

    if (!(constructor->flags & script_function_flag_static))
        parms_size -= 4;

    if (constructor_parms_buffer && parms_size > 0)
        thread->dstack.push(constructor_parms_buffer, parms_size);

    if (constructor_thread)
        *constructor_thread = thread;

    inst->thread_lock.release();
    inst->parent->instance_lock->unlock_ref();
}

// sub_A1CEB0
vm_thread* script_object::add_thread(script_instance* inst, i32 fidx) {
    vm_thread* thread = inst->add_thread(funcs[fidx], nullptr, 0);

    ref_lock_scope    instance_scope(inst->parent->instance_lock);
    engine_lock_scope thread_scope(&inst->thread_lock);

    thread->dstack.push(&inst, sizeof(inst));

    return thread;
}

// sub_A1D300
void script_object::remove_instance(script_instance* delete_me, bool run_destructor_if_present) {
    if ((parent->flags & script_executable_flag_unloading) || (delete_me->flags & script_instance_flag_running_destructor))
        return;

    ref_lock_scope scope(instance_lock);

    delete_me->run_callbacks(script_instance_callback_reason_instance_about_to_die, nullptr);
    delete_me->massacre_threads_by_function(nullptr, nullptr);

    if (run_destructor_if_present && destructor_index >= 0 && (delete_me->flags & script_instance_flag_run_called)) {
        delete_me->flags = (e_script_instance_flags)(delete_me->flags | script_instance_flag_running_destructor);

        add_thread(delete_me, destructor_index);

        // a destructor still running after 0x2000 passes is killed
        i32 runs = 0;

        do {
            delete_me->run(true);

            if (++runs == 0x2000 && delete_me->threads.head) {
                delete_me->kill_thread(delete_me->threads.head);

                break;
            }
        } while (delete_me->threads.head);
    }

    script_instance* inst = instances.head;

    while (inst && inst != delete_me)
        inst = inst->vm_simple_list_next;

    if (!inst)
        return;

    if (inst == global_instance)
        global_instance = nullptr;

    inst->do_garbage_collection();

    instances.erase(inst);

    if (inst) {
        inst->~script_instance();

        references::instance_pool.get().release(inst);
    }
}

// sub_A1CB30
script_instance* script_object::create_auto_instance(f32 argument) {
    ref_lock_scope scope(instance_lock);

    const script_function* constructor = funcs.data[constructor_index];

    if (constructor->parms_stacksize != 4)
        return nullptr;

    script_instance* inst;

    if (flags & script_object_flag_global_object) {
        if (!global_instance) {
            void* memory = references::instance_pool.get().allocate();

            global_instance = memory ?
                new (memory) script_instance("__global", data_blocksize, 0) : nullptr;

            global_instance->parent = this;

            instances.push_back(global_instance);
        }

        inst = global_instance;
    } else {
        void* memory = references::instance_pool.get().allocate();

        inst = memory ?
            new (memory) script_instance("__auto", data_blocksize, 0) : nullptr;

        inst->parent = this;

        instances.push_front(inst);
    }

    vm_thread* thread = inst->add_thread(constructor, nullptr, 0);

    // the global instance gets the argument, an auto instance gets itself
    if (flags & script_object_flag_global_object)
        thread->dstack.push_num(argument);
    else
        thread->dstack.push(&inst, sizeof(inst));

    return inst;
}

// sub_A1D470
void script_object::destruct_instances(bool call_all_destructors) {
    ref_lock_scope scope(instance_lock);

    for (script_instance* inst = instances.head; inst; inst = inst->vm_simple_list_next) {
        if (inst->flags & script_instance_flag_running_destructor)
            continue;

        inst->run_callbacks(script_instance_callback_reason_instance_about_to_die, nullptr);
        inst->massacre_threads_by_function(nullptr, nullptr);
    }

    if (!call_all_destructors || destructor_index < 0)
        return;

    for (script_instance* inst = instances.head; inst; inst = inst->vm_simple_list_next) {
        if ((inst->flags & script_instance_flag_running_destructor) || !(inst->flags & script_instance_flag_run_called))
            continue;

        inst->flags = (e_script_instance_flags)(inst->flags | script_instance_flag_running_destructor);

        add_thread(inst, destructor_index);
    }
}

// sub_A1D630
void script_object::delete_all_instances() {
    ref_lock_scope scope(instance_lock);

    while (instances.head) {
        script_instance* inst = instances.head;

        inst->do_garbage_collection();

        if (inst == global_instance)
            global_instance = nullptr;

        if (instances.head)
            instances.erase(instances.head);

        if (inst) {
            inst->~script_instance();

            references::instance_pool.get().release(inst);
        }
    }
}

// sub_A1D090
void script_object::post_un_mash_fixup(script_executable* requested_parent) {
    parent = requested_parent;

    parent_object = (i32)parent_object == -1 ?
        nullptr : requested_parent->get_object((i32)parent_object);

    i32 count = (i32)funcs.size;

    for (i32 index = 0; index < count; ++index)
        funcs.data[index]->post_un_mash_fixup(this);

    if (flags & script_object_flag_global_object)
        create_auto_instance(0.0f);
}

// sub_A1CF50
void script_object::quick_post_un_mash_fixup() {
    if (flags & script_object_flag_global_object)
        create_auto_instance(0.0f);
}

// sub_A1D520
bool script_object::has_threads() const {
    ref_lock_scope scope(instance_lock);

    for (script_instance* inst = instances.head; inst; inst = inst->vm_simple_list_next) {
        if (inst->threads.head)
            return true;
    }

    return false;
}

// sub_A1D590
void script_object::run(bool ignore_suspended) {
    if (parent->flags & script_executable_flag_suspended_for_script_vars)
        return;

    ref_lock_scope scope(instance_lock);

    flags = (e_script_object_flags)(flags & ~script_object_flag_needs_run);

    script_instance* inst = instances.head;

    while (inst) {
        script_instance* current = inst;

        if (current->threads.head) {
            flags = (e_script_object_flags)(flags | script_object_flag_needs_run);

            current->run(ignore_suspended);
        }

        inst = current->vm_simple_list_next;

        if ((current->flags & script_instance_flag_auto_destruct) && !current->threads.head)
            remove_instance(current, true);
    }
}

// sub_A1C780
i32 script_object::find_func(string_hash func_fullname) const {
    references::function_cache_lock.read()->acquire();

    script_object_function_cache_element* cache = &references::function_cache.get();

    i32 victim       = -1;
    i32 lowest_usage = 0x7FFFFFFF;

    for (i32 slot = 0; slot < 20; ++slot) {
        script_object_function_cache_element &element = cache[slot];

        if (element.function_index == -1) {
            victim = slot;

            break;
        }

        if (element.parent == this && element.function_fullname == func_fullname) {
            element.usage = references::function_cache_usage.get()++;

            i32 index = element.function_index;

            references::function_cache_lock.read()->release();

            return index;
        }

        if (lowest_usage > element.usage) {
            lowest_usage = element.usage;
            victim       = slot;
        }
    }

    i32 found = -1;

    if (flags & script_object_flag_sorted_funcs) {
        if (constructor_index >= 0 && funcs.data[constructor_index]->fullname == func_fullname)
            found = constructor_index;
        else if (destructor_index >= 0 && funcs.data[destructor_index]->fullname == func_fullname)
            found = destructor_index;
        else {
            i32 low = 0;

            if (constructor_index >= 0)
                low = constructor_index + 1;

            if (destructor_index >= 0)
                low = destructor_index + 1;

            i32 high = (i32)funcs.size - 1;
            i32 mid  = (low + high) >> 1;

            script_function* function = funcs.data[mid];

            if (function->fullname == func_fullname)
                found = mid;

            while (found < 0) {
                i32 previous = mid;

                if (function->fullname.source_hash_code >= func_fullname.source_hash_code) {
                    high = mid - 1;

                    if (high < 0)
                        break;
                } else {
                    low = mid + 1;

                    if (low >= (i32)funcs.size)
                        break;
                }

                if (low > high)
                    break;

                mid = (low + high) >> 1;

                if (previous == mid)
                    break;

                function = funcs.data[mid];

                if (function->fullname == func_fullname)
                    found = mid;
            }
        }
    } else {
        for (i32 index = 0; index < (i32)funcs.size; ++index) {
            if (funcs.data[index]->fullname == func_fullname) {
                found = index;

                break;
            }
        }
    }

    if (found >= 0) {
        script_object_function_cache_element &element = cache[victim];

        element.parent            = this;
        element.function_fullname = func_fullname;
        element.function_index    = found;
        element.usage             = references::function_cache_usage.get()++;

        references::function_cache_lock.read()->release();

        return found;
    }

    references::function_cache_lock.read()->release();

    if (!parent_object)
        return -1;

    i32 inherited = parent_object->find_func(func_fullname);

    return inherited == -1 ? -1 : inherited + (i32)funcs.size;
}

// sub_A1CFE0
script_function* script_object::get_function_ptr(u32 index) const {
    const script_object* object = this;

    while (index >= object->funcs.size) {
        index  -= object->funcs.size;
        object  = object->parent_object;
    }

    return object->funcs.data[index];
}

// sub_A1D180
script_instance* script_object::first_instance() const {
    return instances.head;
}
