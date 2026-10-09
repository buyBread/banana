#pragma once

#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class event;

    namespace chuck { namespace vm {
        class script_executable;
        class script_function;
        class script_instance;
    }}

    class arch_base {

    public:
        void**            vtable;
        arch_base_vhandle my_handle;

        // slot 10
        bool is_an_entity_base() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[10])(this);
        }

        void raise_event(string_hash event_type_id);
        void raise_event(event* raised_event);

        u32 add_callback(      string_hash                 event_type_id,
                               chuck::vm::script_instance* instance,
                               chuck::vm::script_function* function,
                         const void*                       parameters,
                               bool                        one_shot);

        void clear_script_callbacks(chuck::vm::script_executable* script_exe);
        void clear_script_callback(string_hash function_name);
        void clear_script_callback(u32 id);
    };

    ASSERT_SIZEOF  (arch_base,            0x08);
    ASSERT_OFFSETOF(arch_base, my_handle, 0x04);
} // treyarch
