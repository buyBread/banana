#pragma once

#include "treyarch/shared/mutex.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_executable;

    namespace fn {
        inline auto object_find_function_by_index =
            (script_function*(__thiscall*)
            (script_object*, u32))
            0x00A1CFE0;
    }

    struct script_object {
        u32                       name_hash;
        script_object*            parent;
        script_executable*        executable;
        script_instance*          global_instance;
        u32                       instance_data_size;
        u32                       function_container;
        u32                       function_count;
        script_function**         functions;
        u32                       function_capacity;
        u32                       function_state;
        i32                       constructor_index;
        i32                       destructor_index;
        u8                        unk_30[0x18];
        script_instance*          first_instance;
        script_instance*          last_instance;
        u32                       instance_count;
        u32                       flags; //  0x1: [chuckvm] ( m_flags & SCRIPT_INSTANCE_FLAG_RUN_CALLED ) == 0
                                         // 0x20: [chuckvm] !( is_flagged( SCRIPT_INSTANCE_FLAG_RUNNING_CALLBACKS ) )
        ref_counted_simple_mutex* instance_lock;

        script_function* function(u32 index) const {
            if (!functions || index >= function_count)
                return nullptr;

            return functions[index];
        }

        script_function* find_function(u32 flattened_index) {
            return fn::object_find_function_by_index(this, flattened_index);
        }
    };
}}} // treyarch::chuck::vm