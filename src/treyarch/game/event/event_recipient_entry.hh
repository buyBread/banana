#pragma once

#include <cstddef>

#include "treyarch/game/event/event_callback.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        struct script_executable;
        struct script_function;
        struct script_instance;
    }}

    class event_recipient_entry {

    public:
        arch_base_vhandle                 recipient_vhandle;
        dinkumware::list<event_callback*> callbacks;
        engine_recursive_lock             lock;

        explicit event_recipient_entry(arch_base_vhandle recipient);
        ~event_recipient_entry();

        void* operator new(std::size_t size);
        void  operator delete(void* allocation) noexcept;

        void clear_callbacks();
        void clear_stale_callbacks();
        void clear_script_callbacks(chuck::vm::script_executable* executable);
        void clear_script_callback(string_hash function_name);
        void clear_script_callback(u32 callback_id);

        void remove_callback(u32 callback_id);

        u32 add_callback(code_event_callback_function function,
                         void*                        parameters,
                         bool                         one_shot);
        u32 add_callback(      chuck::vm::script_instance* instance,
                               chuck::vm::script_function* function,
                         const void*                       parameters,
                               bool                        one_shot);

        bool empty() const noexcept { return callbacks.empty(); }
        u32  callback_count() const noexcept { return callbacks.size(); }
        
        arch_base_vhandle recipient() const noexcept { return recipient_vhandle; }

        dinkumware::list<event_callback*>* callback_list() noexcept { return &callbacks; }
    };

    ASSERT_SIZEOF  (event_recipient_entry,                    0x20);
    ASSERT_OFFSETOF(event_recipient_entry, recipient_vhandle, 0x00);
    ASSERT_OFFSETOF(event_recipient_entry, callbacks,         0x04);
    ASSERT_OFFSETOF(event_recipient_entry, lock,              0x10);
} // treyarch
