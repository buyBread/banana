#pragma once

#include "treyarch/game/event/event_callback.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/types.hh"

namespace treyarch {
    class event;
    class event_recipient_entry;
    class event_type;

    namespace chuck { namespace vm {
        struct script_executable;
        struct script_function;
        struct script_instance;
    }}

    class event_manager {

    public:
        static event_type* find_event_type(string_hash event_type_id);

        static void clear_script_callbacks(arch_base_vhandle recipient, chuck::vm::script_executable* executable);
        static void clear_script_callback(arch_base_vhandle recipient, string_hash function_name);
        static void clear_script_callback(arch_base_vhandle recipient, u32 callback_id);

        static void remove_callback(u32 callback_id, string_hash event_type_id, arch_base_vhandle recipient);
        static bool has_callbacks(arch_base_vhandle recipient, string_hash event_type_id);

        static void raise_event(string_hash event_type_id, arch_base_vhandle recipient);
        static void raise_event(event* raised_event, arch_base_vhandle recipient);

        static void clear();

        static event_type* register_event_type(string_hash event_type_id);

        static void garbage_collect();

        static void create_inst();
        static void delete_inst();

        static event_recipient_entry* create_event_recipient(string_hash event_type_id, arch_base_vhandle recipient);

        static u32 add_callback(string_hash                  event_type_id,
                                arch_base_vhandle            recipient,
                                code_event_callback_function function,
                                void*                        parameters,
                                bool                         one_shot);
        static u32 add_callback(      string_hash                 event_type_id,
                                      arch_base_vhandle           recipient,
                                      chuck::vm::script_instance* instance,
                                      chuck::vm::script_function* function,
                                const void*                       parameters,
                                      bool                        one_shot);
        static u32 add_default_callback(string_hash                  event_type_id,
                                        code_event_callback_function function,
                                        void*                        parameters,
                                        bool                         one_shot);
    };
} // treyarch
