#include <cstring>

#include "retail.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/game/event/event.hh"
#include "treyarch/game/event/event_callback.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<u32>                     callback_id_counter { 0x0102C230 };
    util::memory_reference<mash::virtual_types_key> chuck_event_type    { 0x010F7160 };
    util::memory_reference<mash::virtual_types_key> data_event_type     { 0x010F7188 };
}} // treyarch::references

using namespace treyarch;

// inlined into the subclass constructors (sub_683980, sub_683E20)
event_callback::event_callback(void* requested_parameters,
                               bool  requested_one_shot) : parameters(requested_parameters),
                                                           id(0),
                                                           disabled(false),
                                                           one_shot(requested_one_shot),
                                                           padding_0e {} {

    id = ++references::callback_id_counter.get();

    if (!id)
        id = ++references::callback_id_counter.get();
}

// inlined into the deleting destructors (sub_683DC0, sub_683EF0)
void event_callback::operator delete(void* allocation) noexcept {
    memory::heap::free(allocation);
}

// sub_683980
code_event_callback::code_event_callback(code_event_callback_function requested_function,
                                         void*                        requested_parameters,
                                         bool                         requested_one_shot) : event_callback(requested_parameters, requested_one_shot),
                                                                                            function(requested_function) {}

// sub_6839E0
void code_event_callback::spawn(event* raised_event, arch_base_vhandle recipient) {
    function(raised_event, recipient, parameters);
}

// sub_683E20
script_event_callback::script_event_callback(      chuck::vm::script_instance* requested_instance,
                                                   chuck::vm::script_function* requested_function,
                                             const void*                       requested_parameters,
                                                   bool                        requested_one_shot) : event_callback((void*)requested_parameters, requested_one_shot),
                                                                                                     instance(requested_instance),
                                                                                                     function(requested_function) {

    u32 size = function->parms_stacksize;

    if (size) {
        parameters = memory::heap::allocate(size);
        std::memcpy(parameters, requested_parameters, size);
        retail::sub_A20440((u32*)function, 0, (i32)parameters, size, 1, 0); // script_function: retain argument references
    } else
        parameters = nullptr;

    retail::sub_A1DDC0((u32*)instance, (i32)&script_event_callback::on_instance_lifecycle, (i32)this); // script_instance::register_callback
}

// sub_683A00
script_event_callback::~script_event_callback() {
    if (references::g_world_ptr.read() && instance) {
        retail::sub_A1E160((u32*)instance, (i32)this); // script_instance::unregister_callback

        if (parameters)
            retail::sub_A20560((u32*)function, (i32)parameters, function->parms_stacksize); // script_function: release argument references
    }

    if (parameters)
        memory::heap::free(parameters);
}

// sub_683DE0
void script_event_callback::on_instance_lifecycle(chuck::vm::e_script_instance_callback_reason reason,
                                                  chuck::vm::script_instance*,
                                                  chuck::vm::vm_thread*,
                                                  void*                                        user_data) {

    if (reason)
        return;

    script_event_callback* callback = (script_event_callback*)user_data;

    if (callback->instance && callback->parameters && callback->function) {
        retail::sub_A20560((u32*)callback->function, // script_function: release argument references
                           (i32)callback->parameters,
                           callback->function->parms_stacksize);
    }

    callback->instance = nullptr;
}

// sub_683F10
void script_event_callback::spawn(event* raised_event, arch_base_vhandle) {
    if (disabled || !instance)
        return;

    // script_instance::add_thread
    chuck::vm::vm_thread* thread = (chuck::vm::vm_thread*)retail::sub_A1E0F0((u32*)instance,
                                                                             (i32)function,
                                                                             parameters,
                                                                             function->parms_stacksize,
                                                                             0,
                                                                             0);

    if (raised_event->is_or_is_subclass_of(references::chuck_event_type.read())) {
        i32 size = *(i32*)((u8*)raised_event + 0x10);

        if (size > 0)
            retail::sub_4E4F70((i32)&thread->dstack, (u8*)raised_event + 0x14, size); // vm_stack::push

        return;
    }

    if (!raised_event->is_or_is_subclass_of(references::data_event_type.read()))
        return;

    using get_data_function = const void*(__thiscall*)(event*);
    using get_size_function = i32(__thiscall*)(event*);

    void** vtable = *(void***)raised_event;

    i32 size = ((get_size_function)vtable[13])(raised_event);

    if (size > 0) {
        const void* data = ((get_data_function)vtable[12])(raised_event);

        retail::sub_4E4F70((i32)&thread->dstack, (void*)data, size); // vm_stack::push
    }
}

// sub_6842F0
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
