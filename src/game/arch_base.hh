#pragma once

#include "game/event/event_callback.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"

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

        // slot 11
        bool is_an_entity() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[11])(this);
        }

        // slot 12
        bool is_an_actor() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[12])(this);
        }

        // slot 13
        bool is_a_conglomerate() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[13])(this);
        }

        // slot 14
        bool is_a_conglomerate_clone() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[14])(this);
        }

        // slot 15
        bool is_a_dynamic_conglomerate_clone() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[15])(this);
        }

        // slot 16
        bool is_a_grenade() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[16])(this);
        }

        // slot 18
        bool is_a_particle_generator() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[18])(this);
        }

        // slot 23
        bool is_a_marker() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[23])(this);
        }

        // slot 30
        bool is_a_marky_camera() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[30])(this);
        }

        // slot 41
        bool is_a_vehicle() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[41])(this);
        }

        // slot 45
        bool is_a_fight_group() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[45])(this);
        }

        // slot 46
        bool is_a_fakir() const {
            return ((bool (__thiscall*)(const arch_base*))vtable[46])(this);
        }

        void raise_event(string_hash event_type_id);
        void raise_event(event* raised_event);

        u32 add_callback(      string_hash                 event_type_id,
                               chuck::vm::script_instance* instance,
                               chuck::vm::script_function* function,
                         const void*                       parameters,
                               bool                        one_shot);
        u32 add_callback(string_hash                  event_type_id,
                         code_event_callback_function function,
                         void*                        parameters,
                         bool                         one_shot);

        void remove_callback(u32 callback_id, string_hash event_type_id);

        void clear_script_callbacks(chuck::vm::script_executable* script_exe);
        void clear_script_callback(string_hash function_name);
        void clear_script_callback(u32 id);
    };

    ASSERT_SIZEOF  (arch_base,            0x08);
    ASSERT_OFFSETOF(arch_base, my_handle, 0x04);
} // treyarch
