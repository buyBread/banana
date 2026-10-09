#include <new>
#include <windows.h>

#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A21B60
void vm_thread::register_callbacks(raise_global_signal_callback_t             raise_global_signal,
                                   raise_instance_signal_callback_t           raise_instance_signal,
                                   raise_library_signal_callback_t            raise_library_signal,
                                   clear_global_callback_by_name_callback_t   clear_global_callback_by_name,
                                   clear_global_callback_by_id_callback_t     clear_global_callback_by_id,
                                   clear_instance_callback_by_name_callback_t clear_instance_callback_by_name,
                                   clear_instance_callback_by_id_callback_t   clear_instance_callback_by_id,
                                   add_global_callback_callback_t             add_global_callback,
                                   add_instance_callback_callback_t           add_instance_callback,
                                   add_library_callback_callback_t            add_library_callback) {

    references::raise_global_signal_callback           .write(raise_global_signal);
    references::raise_instance_signal_callback         .write(raise_instance_signal);
    references::raise_library_signal_callback          .write(raise_library_signal);
    references::clear_global_callback_by_name_callback  .write(clear_global_callback_by_name);
    references::clear_global_callback_by_id_callback    .write(clear_global_callback_by_id);
    references::clear_instance_callback_by_name_callback.write(clear_instance_callback_by_name);
    references::clear_instance_callback_by_id_callback  .write(clear_instance_callback_by_id);
    references::add_global_callback_callback           .write(add_global_callback);
    references::add_instance_callback_callback         .write(add_instance_callback);
    references::add_library_callback_callback          .write(add_library_callback);
}

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
