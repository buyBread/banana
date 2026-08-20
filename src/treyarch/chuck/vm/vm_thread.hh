#pragma once

#include "treyarch/chuck/vm/opcodes.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct vm_thread_local_reference;
    struct script_instance;

    enum class e_push_pop_modifier : u32 {
        NONE           = 0,
        INLINE_ARRAY   = 1,
        INDIRECT_ARRAY = 2,
        ADDRESS        = 3,
        DYNAMIC_ARRAY  = 4
    };

    // milestone name: vm_thread_flow_stack_element
    // "flow_frame" is semantically cleaner
    struct vm_flow_frame {
        u8*              return_pc;
        script_instance* previous_current_instance;
        vm_flow_frame*   previous;
    };

    union vm_argument {
        u8  raw[0x0c];
        i16 word;

        struct {
            i16 word_1,
                word_2;
        } word_pair;

        f32   number;
        u32   unsigned_value;
        void* pointer;
    };

    struct vm_thread {
        script_instance*           instance;
        script_function*           entry_function;
        vm_thread*                 creator;
        vm_stack                   stack;
        u8*                        pc;
        e_push_pop_modifier        next_push_pop_modifier;
        vm_argument                next_push_pop_arg;
        f32                        next_push_pop_subscript;
        vm_flow_frame*             flow_frame;
        e_opcode                   current_opcode;
        u32                        current_arg_type;
        vm_argument                current_arg;
        u32                        current_dsize;
        script_instance*           current_instance;
        u32                        native_recall;
        void*                      constructor_context;
        u32                        unk_60;
        f32                        unk_64;
        u32                        id;
        vm_thread_local_reference* local_references;
        u32                        unk_70;
        u32                        unk_74;
        vm_thread**                thread_list_owner;
        vm_thread*                 previous;
        vm_thread*                 next;
    };
}}} // treyarch::chuck::vm