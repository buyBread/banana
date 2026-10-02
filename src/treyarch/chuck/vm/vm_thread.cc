#include <new>
#include <windows.h>

#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A22140
vm_thread::vm_thread(      script_instance* instance,
                     const script_function* function,
                           void*            requested_user_data,
                           u32              stack_size) : inst(instance),
                                                          ex(function),
                                                          creator(nullptr),
                                                          dstack(this, stack_size),
                                                          pc(function->buffer),
                                                          flow_stack(nullptr),
                                                          current_instance(instance),
                                                          entry(0),
                                                          user_data(requested_user_data),
                                                          unk_60(0),
                                                          camera_priority(0.0f),
                                                          local_references { nullptr, nullptr, 0 },
                                                          thread_list(nullptr),
                                                          vm_simple_list_previous(nullptr),
                                                          vm_simple_list_next(nullptr) {

    thread_id = _InterlockedIncrement((volatile LONG*)&references::id_counter.get());
}

// sub_A21FE0
void vm_thread::add_local_reference(void* allocation, u32 mode) {
    if (!allocation)
        return;

    void* memory = references::local_reference_pool.get().allocate();

    vm_thread_local_reference* reference = memory ?
        new (memory) vm_thread_local_reference(allocation, mode | 1) : nullptr;

    reference->list                    = &local_references;
    reference->vm_simple_list_previous = local_references.last;
    reference->vm_simple_list_next     = nullptr;

    if (local_references.last)
        local_references.last->vm_simple_list_next = reference;

    bool was_empty = !local_references.head;

    local_references.last = reference;

    if (was_empty)
        local_references.head = reference;

    ++local_references.size;
}
