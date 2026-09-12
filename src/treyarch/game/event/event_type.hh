#pragma once

#include <cstddef>

#include "treyarch/game/event/event_callback.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/dinkumware/hash_map.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class event;
    class event_recipient_entry;

    namespace chuck { namespace vm {
        struct script_executable;
        struct script_function;
        struct script_instance;
    }}

    class event_type {

        event_recipient_entry*                          cached_event_recipient_entry;
        string_hash                                     event_type_id;
        event*                                          event_to_raise;
        u32                                             padding_0c;
        engine_recursive_lock                           lock;
        dinkumware::hash_map
            <arch_base_vhandle, event_recipient_entry*> recipients;
        dinkumware::list<event_callback*>               default_callbacks;
        u32                                             padding_54;

    public:
        explicit event_type(string_hash event_type_id);

        ~event_type();

        void* operator new(std::size_t size);
        void  operator delete(void* allocation) noexcept;

        void clear();
        void clear_callbacks();
        void clear_script_callbacks(arch_base_vhandle recipient, chuck::vm::script_executable* executable);
        void clear_script_callback(arch_base_vhandle recipient, string_hash function_name);
        void clear_script_callback(arch_base_vhandle recipient, u32 callback_id);

        event_recipient_entry* find_recipient_entry(arch_base_vhandle recipient);
        event_recipient_entry* create_recipient_entry(arch_base_vhandle recipient);

        void remove_default_callback(u32 callback_id);
        bool has_callbacks(arch_base_vhandle recipient);
        void raise_event(arch_base_vhandle recipient, event* external_event = nullptr);

        u32 add_default_callback(code_event_callback_function function, void* parameters, bool one_shot);

        bool garbage_collect();

        string_hash type_id() const noexcept { return event_type_id; }
    };

    ASSERT_SIZEOF(event_type, 0x58);
} // treyarch
