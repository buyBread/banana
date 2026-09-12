#include <new>

#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/game/event/event_pools.hh"
#include "treyarch/game/event/event_recipient_entry.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

event_recipient_entry::event_recipient_entry
    (arch_base_vhandle requested_recipient) : recipient_vhandle(requested_recipient),
                                              callbacks(),
                                              lock {} {}

event_recipient_entry::~event_recipient_entry() {
    recipient_vhandle = arch_base_vhandle();
    
    clear_callbacks();
}

void* event_recipient_entry::operator new(std::size_t) {
    return event_pools::recipient_pool().allocate();
}

void event_recipient_entry::operator delete(void* allocation) noexcept {
    event_pools::recipient_pool().release(allocation);
}

void event_recipient_entry::clear_callbacks() {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end();) {
        delete position->value;
        position = callbacks.erase(position);
    }
}

void event_recipient_entry::clear_stale_callbacks() {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end();) {
        event_callback* callback = position->value;

        if (callback->is_script_callback() &&
            !((script_event_callback*)callback)->script_instance()) {

            delete callback;
            position = callbacks.erase(position);
        } else
            position = position->next;
    }
}

void event_recipient_entry::clear_script_callbacks(chuck::vm::script_executable* executable) {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end();) {
        event_callback* callback = position->value;
        bool remove = false;

        if (callback->is_script_callback()) {
            auto* script_callback = (script_event_callback*)callback;
            auto* instance        = script_callback->script_instance();

            remove = instance &&
                     (!executable || instance->object->executable == executable);
        }

        if (remove) {
            delete callback;
            position = callbacks.erase(position);
        } else
            position = position->next;
    }
}

void event_recipient_entry::clear_script_callback(string_hash function_name) {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end();) {
        event_callback* callback = position->value;
        bool remove = false;

        if (callback->is_script_callback()) {
            auto* script_callback = (script_event_callback*)callback;

            remove = script_callback->script_instance() &&
                     script_callback->script_function()->signature_hash == function_name.source_hash_code;
        }

        if (remove) {
            delete callback;
            position = callbacks.erase(position);
        } else
            position = position->next;
    }
}

void event_recipient_entry::clear_script_callback(u32 callback_id) {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end(); position = position->next) {
        event_callback* callback = position->value;

        if (callback->is_script_callback() &&
            ((script_event_callback*)callback)->script_instance() &&
            callback->callback_id() == callback_id) {

            delete callback;
            callbacks.erase(position);

            break;
        }
    }
}

void event_recipient_entry::remove_callback(u32 callback_id) {
    engine_lock_scope scope(&lock);

    for (auto* position = callbacks.begin(); position != callbacks.end();) {
        event_callback* callback = position->value;

        if (callback->callback_id() == callback_id) {
            delete callback;
            position = callbacks.erase(position);
        } else
            position = position->next;
    }
}

u32 event_recipient_entry::add_callback(code_event_callback_function function,
                                        void*                        parameters,
                                        bool                         one_shot) {

    auto* allocation = memory::heap::allocate(sizeof(code_event_callback));
    auto* callback   = new (allocation) code_event_callback(function, parameters, one_shot);

    engine_lock_scope scope(&lock);

    callbacks.push_back(callback);
    
    return callback->callback_id();
}

u32 event_recipient_entry::add_callback(      chuck::vm::script_instance* instance,
                                              chuck::vm::script_function* function,
                                        const void*                       parameters,
                                              bool                        one_shot) {
                                            
    auto* allocation = memory::heap::allocate(sizeof(script_event_callback));
    auto* callback   = new (allocation) script_event_callback(instance, function, parameters, one_shot);

    engine_lock_scope scope(&lock);

    callbacks.push_back(callback);

    return callback->callback_id();
}
