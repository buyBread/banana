#include <cstring>

#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/game/event/event.hh"
#include "treyarch/game/event/event_callback.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

event_callback::event_callback(void* requested_parameters,
                               bool  requested_one_shot) : parameters(requested_parameters),
                                                           id(0),
                                                           disabled(false),
                                                           one_shot(requested_one_shot),
                                                           padding_0e {} {

    static util::memory_reference<u32> id_counter { 0x0102C230 };

    id = ++id_counter.get();

    if (!id)
        id = ++id_counter.get();
}

void event_callback::operator delete(void* allocation) noexcept {
    memory::heap::free(allocation);
}

code_event_callback::code_event_callback(code_event_callback_function requested_function,
                                         void*                        requested_parameters,
                                         bool                         requested_one_shot) : event_callback(requested_parameters, requested_one_shot),
                                                                                            function(requested_function) {}

void code_event_callback::spawn(event* raised_event, arch_base_vhandle recipient) {
    function(raised_event, recipient, parameters);
}

script_event_callback::script_event_callback(      chuck::vm::script_instance* requested_instance,
                                                   chuck::vm::script_function* requested_function,
                                             const void*                       requested_parameters,
                                                   bool                        requested_one_shot) : event_callback((void*)requested_parameters, requested_one_shot),
                                                                                                     instance(requested_instance),
                                                                                                     function(requested_function) {

    u32 size = function->argument_size();

    if (size) {
        parameters = memory::heap::allocate(size);
        std::memcpy(parameters, requested_parameters, size);
        function->retain_argument_references(parameters, size);
    } else
        parameters = nullptr;

    instance->add_lifecycle_callback(&script_event_callback::on_instance_lifecycle, this);
}

script_event_callback::~script_event_callback() {
    static util::memory_reference<void*> game_state { 0x010FC54C };

    if (game_state.read() && instance) {
        instance->remove_lifecycle_callback(this);

        if (parameters)
            function->release_argument_references(parameters, function->argument_size());
    }

    if (parameters)
        memory::heap::free(parameters);
}

void __cdecl script_event_callback::on_instance_lifecycle(i32                          reason,
                                                          chuck::vm::script_instance*,
                                                          chuck::vm::vm_thread*,
                                                          void*                        user_data) {
    if (reason)
        return;

    script_event_callback* callback = (script_event_callback*)user_data;

    if (callback->instance && callback->parameters && callback->function) {
        callback->function->release_argument_references(callback->parameters,
                                                        callback->function->argument_size());
    }

    callback->instance = nullptr;
}

void script_event_callback::spawn(event* raised_event, arch_base_vhandle) {
    if (disabled || !instance)
        return;

    chuck::vm::vm_thread* thread =
        chuck::vm::fn::instance_add_thread_with_arguments(instance,
                                                          function,
                                                          parameters,
                                                          function->argument_size(),
                                                          nullptr,
                                                          0);

    static util::memory_reference<mash::virtual_types_key> chuck_event_type { 0x010F7160 };
    static util::memory_reference<mash::virtual_types_key> data_event_type  { 0x010F7188 };

    if (raised_event->is_or_is_subclass_of(chuck_event_type.read())) {
        i32 size = *(i32*)((u8*)raised_event + 0x10);

        if (size > 0)
            chuck::vm::fn::stack_push_bytes(&thread->stack, (u8*)raised_event + 0x14, size);

        return;
    }

    if (!raised_event->is_or_is_subclass_of(data_event_type.read()))
        return;

    using get_data_function = const void*(__thiscall*)(event*);
    using get_size_function = i32(__thiscall*)(event*);

    void** vtable = *(void***)raised_event;

    i32 size = ((get_size_function)vtable[13])(raised_event);

    if (size > 0) {
        const void* data = ((get_data_function)vtable[12])(raised_event);

        chuck::vm::fn::stack_push_bytes(&thread->stack, data, size);
    }
}

void treyarch::dispatch_event_callbacks(event*                             raised_event,
                                        arch_base_vhandle                  recipient,
                                        dinkumware::list<event_callback*>* callbacks) {

    using callback_list = dinkumware::list<event_callback*>;

    callback_list::node* position = callbacks->begin();

    while (position != callbacks->end()) {
        event_callback* callback = position->value;

        if (!callback->is_disabled()) {
            if (recipient.null() || recipient.resolve())
                callback->spawn(raised_event, recipient);

            if (!callback->is_disabled() && callback->is_one_shot()) {
                delete callback;

                position = callbacks->erase(position);

                continue;
            }
        }

        position = position->next;
    }
}
