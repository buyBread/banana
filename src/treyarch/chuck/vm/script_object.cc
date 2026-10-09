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
