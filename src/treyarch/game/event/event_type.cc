#include <new>

#include "treyarch/game/event/event.hh"
#include "treyarch/game/event/event_pools.hh"
#include "treyarch/game/event/event_recipient_entry.hh"
#include "treyarch/game/event/event_type.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

event_type::event_type(string_hash requested_event_type_id) : cached_event_recipient_entry(nullptr),
                                                              event_type_id(),
                                                              event_to_raise(nullptr),
                                                              padding_0c(0),
                                                              lock {},
                                                              recipients(),
                                                              default_callbacks(),
                                                              padding_54(0) {

    clear();

    event_type_id = requested_event_type_id;
    event_to_raise = new event(event_type_id, true);
}

event_type::~event_type() {
    clear();
    
    delete event_to_raise;
}

void* event_type::operator new(std::size_t) {
    return event_pools::event_type_pool().allocate();
}

void event_type::operator delete(void* allocation) noexcept {
    event_pools::event_type_pool().release(allocation);
}

void event_type::clear() {
    event_type_id = string_hash();

    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end(); position = position->next)
        delete position->value.value;

    recipients.clear();
    cached_event_recipient_entry = nullptr;
    clear_callbacks();
}

void event_type::clear_callbacks() {
    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end(); position = position->next)
        position->value.value->clear_callbacks();

    for (auto* position = default_callbacks.begin(); position != default_callbacks.end();) {
        delete position->value;
        position = default_callbacks.erase(position);
    }
}

void event_type::clear_script_callbacks(arch_base_vhandle             recipient,
                                        chuck::vm::script_executable* executable) {

    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end(); position = position->next) {
        if (position->value.key == recipient)
            position->value.value->clear_script_callbacks(executable);
    }
}

void event_type::clear_script_callback(arch_base_vhandle recipient, string_hash function_name) {
    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end(); position = position->next) {
        if (position->value.key == recipient)
            position->value.value->clear_script_callback(function_name);
    }
}

void event_type::clear_script_callback(arch_base_vhandle recipient, u32 callback_id) {
    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end(); position = position->next) {
        if (position->value.key == recipient)
            position->value.value->clear_script_callback(callback_id);
    }
}

event_recipient_entry* event_type::find_recipient_entry(arch_base_vhandle recipient) {
    engine_lock_scope scope(&lock);

    if (cached_event_recipient_entry && cached_event_recipient_entry->recipient() == recipient)
        return cached_event_recipient_entry;

    auto* position = recipients.find(recipient);

    if (position == recipients.end())
        return nullptr;

    cached_event_recipient_entry = position->value.value;

    return cached_event_recipient_entry;
}

event_recipient_entry* event_type::create_recipient_entry(arch_base_vhandle recipient) {
    engine_lock_scope scope(&lock);

    event_recipient_entry* entry = find_recipient_entry(recipient);

    if (entry)
        return entry;

    entry = new event_recipient_entry(recipient);
    recipients.insert(recipient, entry);

    return entry;
}

void event_type::remove_default_callback(u32 callback_id) {
    engine_lock_scope scope(&lock);

    for (auto* position = default_callbacks.begin(); position != default_callbacks.end();) {
        event_callback* callback = position->value;

        if (callback->callback_id() == callback_id) {
            delete callback;
            position = default_callbacks.erase(position);
        } else
            position = position->next;
    }
}

bool event_type::has_callbacks(arch_base_vhandle recipient) {
    engine_lock_scope scope(&lock);

    if (!default_callbacks.empty())
        return true;

    event_recipient_entry* entry = find_recipient_entry(recipient);

    return entry && !entry->empty();
}

void event_type::raise_event(arch_base_vhandle recipient, event* external_event) {
    event* raised_event = external_event ? external_event : event_to_raise;

    engine_lock_scope scope(&lock);

    event_recipient_entry* entry = find_recipient_entry(recipient);

    if (entry) {
        if ((entry->recipient().null() || entry->recipient().resolve()) && !entry->empty())
            entry->clear_stale_callbacks();

        raised_event->raise();
        dispatch_event_callbacks(raised_event, recipient, entry->callback_list());
    }

    if (!default_callbacks.empty())
        dispatch_event_callbacks(raised_event, recipient, &default_callbacks);
}

u32 event_type::add_default_callback(code_event_callback_function function,
                                     void*                        parameters,
                                     bool                         one_shot) {

    auto* allocation = memory::heap::allocate(sizeof(code_event_callback));
    auto* callback   = new (allocation) code_event_callback(function, parameters, one_shot);

    engine_lock_scope scope(&lock);

    default_callbacks.push_back(callback);

    return callback->callback_id();
}

bool event_type::garbage_collect() {
    engine_lock_scope scope(&lock);

    for (auto* position = recipients.begin(); position != recipients.end();) {
        event_recipient_entry* entry = position->value.value;
        
        bool keep = (entry->recipient().null() || entry->recipient().resolve()) && !entry->empty();

        if (keep) {
            entry->clear_stale_callbacks();
            keep = !entry->empty();
        }

        if (!keep) {
            if (cached_event_recipient_entry == entry)
                cached_event_recipient_entry = nullptr;

            delete entry;
            position = recipients.erase(position);
        } else
            position = position->next;
    }

    return recipients.empty() && default_callbacks.empty();
}
