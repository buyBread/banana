#include <cstdlib>

#include "treyarch/game/event/event.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/event/event_pools.hh"
#include "treyarch/game/event/event_recipient_entry.hh"
#include "treyarch/game/event/event_type.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/mutex.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace event_manager_references {
    util::memory_reference<bool>                            initialized       { 0x0102C234 };
    util::memory_reference<i32>                             garbage_index     { 0x0102C238 };
    util::memory_reference<event_type*>                     cached_event_type { 0x0102C23C };
    util::memory_reference<engine_recursive_lock>           lock              { 0x0102C6E0 };
    util::memory_reference<dinkumware::vector<event_type*>> event_types       { 0x0102CC94 };
}} // treyarch::event_manager_references

namespace treyarch { namespace event_manager_functions {
    int __cdecl compare_event_types(const void* left, const void* right) {
        const event_type* left_type  = *(event_type* const*)left;
        const event_type* right_type = *(event_type* const*)right;

        u32 left_id  = left_type->type_id().source_hash_code;
        u32 right_id = right_type->type_id().source_hash_code;

        return left_id < right_id ? -1 : left_id != right_id;
    }
}} // treyarch::event_manager_functions

using namespace treyarch;

event_type* event_manager::find_event_type(string_hash event_type_id) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* cached = event_manager_references::cached_event_type.read();

    if (cached && cached->type_id() == event_type_id)
        return cached;

    auto &event_types = event_manager_references::event_types.get();
    u32   first       = 0;
    u32   count       = event_types.size();

    while (count) {
        u32 step      = count / 2;
        u32 index     = first + step;
        u32 candidate = event_types[index]->type_id().source_hash_code;

        if (candidate < event_type_id.source_hash_code) {
            first = index + 1;
            count -= step + 1;
        } else
            count = step;
    }

    if (first == event_types.size() || event_types[first]->type_id() != event_type_id)
        return nullptr;

    cached = event_types[first];
    event_manager_references::cached_event_type.write(cached);

    return cached;
}

void event_manager::clear_script_callbacks(arch_base_vhandle             recipient,
                                           chuck::vm::script_executable* executable) {

    engine_lock_scope scope(&event_manager_references::lock.get());

    for (event_type* type : event_manager_references::event_types.get())
        type->clear_script_callbacks(recipient, executable);
}

void event_manager::clear_script_callback(arch_base_vhandle recipient, string_hash function_name) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    for (event_type* type : event_manager_references::event_types.get())
        type->clear_script_callback(recipient, function_name);
}

void event_manager::clear_script_callback(arch_base_vhandle recipient, u32 callback_id) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    for (event_type* type : event_manager_references::event_types.get())
        type->clear_script_callback(recipient, callback_id);
}

void event_manager::remove_callback(u32               callback_id,
                                    string_hash       event_type_id,
                                    arch_base_vhandle recipient) {

    if (!event_manager_references::initialized.read())
        return;

    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = find_event_type(event_type_id);

    if (!type)
        return;

    type->remove_default_callback(callback_id);

    event_recipient_entry* entry = type->find_recipient_entry(recipient);

    if (entry)
        entry->remove_callback(callback_id);
}

bool event_manager::has_callbacks(arch_base_vhandle recipient, string_hash event_type_id) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = find_event_type(event_type_id);

    return type && type->has_callbacks(recipient);
}

void event_manager::raise_event(string_hash event_type_id, arch_base_vhandle recipient) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = find_event_type(event_type_id);

    if (type)
        type->raise_event(recipient);
}

void event_manager::raise_event(event* raised_event, arch_base_vhandle recipient) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = find_event_type(raised_event->type_id());

    if (type)
        type->raise_event(recipient, raised_event);
}

void event_manager::clear() {
    engine_lock_scope scope(&event_manager_references::lock.get());

    auto &event_types = event_manager_references::event_types.get();

    for (event_type* type : event_types)
        delete type;

    event_types.clear();
    event_manager_references::garbage_index.write(0);
    event_manager_references::cached_event_type.write(nullptr);
}

event_type* event_manager::register_event_type(string_hash event_type_id) {
    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = find_event_type(event_type_id);

    if (type)
        return type;

    type = new event_type(event_type_id);

    auto &event_types = event_manager_references::event_types.get();
    event_types.push_back(type);

    std::qsort(event_types.begin(),
               event_types.size(),
               sizeof(event_type*),
               &event_manager_functions::compare_event_types);
    
    return type;
}

void event_manager::garbage_collect() {
    engine_lock_scope scope(&event_manager_references::lock.get());

    auto &event_types = event_manager_references::event_types.get();
    i32  &index       = event_manager_references::garbage_index.get();

    if (event_types.empty() || (u32)index >= event_types.size()) {
        index = 0;

        return;
    }

    event_type** position = event_types.begin() + index;
    event_type*  type     = *position;

    if (type->garbage_collect()) {
        if (type == event_manager_references::cached_event_type.read())
            event_manager_references::cached_event_type.write(nullptr);

        delete type;
        event_types.erase(position);
    } else
        ++index;
}

void event_manager::create_inst() {
    clear();

    event_pools::recipient_pool() .initialize("event_recipient_entry", 0x20, 4, 512, 3);
    event_pools::event_type_pool().initialize("event_type",            0x58, 4, 256, 5);
    event_pools::event_pool()     .initialize("event",                 0x10, 4, 256, 5);

    delete new event_type(string_hash());
    delete new event_recipient_entry(arch_base_vhandle());

    event_manager_references::initialized.write(true);
}

void event_manager::delete_inst() {
    event_manager_references::initialized.write(false);

    clear();
}

event_recipient_entry* event_manager::create_event_recipient(string_hash       event_type_id,
                                                             arch_base_vhandle recipient) {

    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = register_event_type(event_type_id);

    return type ?
        type->create_recipient_entry(recipient) : nullptr;
}

u32 event_manager::add_callback(string_hash                  event_type_id,
                                arch_base_vhandle            recipient,
                                code_event_callback_function function,
                                void*                        parameters,
                                bool                         one_shot) {

    engine_lock_scope scope(&event_manager_references::lock.get());

    event_recipient_entry* entry = create_event_recipient(event_type_id, recipient);

    return entry ?
        entry->add_callback(function, parameters, one_shot) : 0;
}

u32 event_manager::add_callback(      string_hash                 event_type_id,
                                      arch_base_vhandle           recipient,
                                      chuck::vm::script_instance* instance,
                                      chuck::vm::script_function* function,
                                const void*                       parameters,
                                      bool                        one_shot) {

    engine_lock_scope scope(&event_manager_references::lock.get());

    event_recipient_entry* entry = create_event_recipient(event_type_id, recipient);

    return entry ?
        entry->add_callback(instance, function, parameters, one_shot) : 0;
}

u32 event_manager::add_default_callback(string_hash                  event_type_id,
                                        code_event_callback_function function,
                                        void*                        parameters,
                                        bool                         one_shot) {

    engine_lock_scope scope(&event_manager_references::lock.get());

    event_type* type = register_event_type(event_type_id);

    return type ?
        type->add_default_callback(function, parameters, one_shot) : 0;
}
