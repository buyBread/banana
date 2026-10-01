#pragma once

#include "treyarch/chuck/vm/opcodes.hh"
#include "treyarch/chuck/vm/vm_simple_list.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_function;
    class script_instance;
    class vm_thread;
    struct vm_thread_local_reference; // at least { tracked allocation, mode | 1 }

    /*  game-side event services the interpreter calls (installed by sub_843300).
        "library" signallers are native objects, i.e. entity handles. */
    using raise_global_signal_callback_t = void(*)(vm_thread*  source_thread,
                                                   string_hash signal,
                                                   void*       raise_args,
                                                   u32         args_stack_size);
    using raise_instance_signal_callback_t = void(*)(vm_thread*       source_thread,
                                                     string_hash      signal,
                                                     script_instance* inst,
                                                     void*            raise_args,
                                                     u32              args_stack_size);
    using raise_library_signal_callback_t = void(*)(vm_thread*        source_thread,
                                                    string_hash       signal,
                                                    arch_base_vhandle signaller,
                                                    void*             raise_args,
                                                    u32               args_stack_size);

    using clear_global_callback_by_name_callback_t = void(*)(vm_thread*  source_thread,
                                                             string_hash name);
    using clear_global_callback_by_id_callback_t = void(*)(vm_thread* source_thread,
                                                           u32        id);
    using clear_instance_callback_by_name_callback_t = void(*)(vm_thread*       source_thread,
                                                               string_hash      name,
                                                               script_instance* inst);
    using clear_instance_callback_by_id_callback_t = void(*)(vm_thread*       source_thread,
                                                             u32              id,
                                                             script_instance* inst);

    using add_global_callback_callback_t = u32(*)(vm_thread*       source_thread,
                                                  string_hash      signal,
                                                  script_instance* inst,
                                                  script_function* sfr,
                                                  const void*      parms,
                                                  bool             one_shot);
    using add_instance_callback_callback_t = u32(*)(vm_thread*       source_thread,
                                                    string_hash      signal,
                                                    script_instance* me,
                                                    script_instance* inst,
                                                    script_function* sfr,
                                                    const void*      parms,
                                                    bool             one_shot);
    using add_library_callback_callback_t = u32(*)(vm_thread*        source_thread,
                                                   string_hash       signal,
                                                   arch_base_vhandle signaller,
                                                   script_instance*  inst,
                                                   script_function*  sfr,
                                                   const void*       parms,
                                                   bool              one_shot);

    namespace references {
        inline util::memory_reference<u32> id_counter { 0x01124C70 };

        inline util::memory_reference<raise_global_signal_callback_t>   raise_global_signal_callback   { 0x01124C74 };
        inline util::memory_reference<raise_instance_signal_callback_t> raise_instance_signal_callback { 0x01124C78 };
        inline util::memory_reference<raise_library_signal_callback_t>  raise_library_signal_callback  { 0x01124C7C };

        inline util::memory_reference<clear_global_callback_by_name_callback_t>   clear_global_callback_by_name_callback   { 0x01124C80 };
        inline util::memory_reference<clear_global_callback_by_id_callback_t>     clear_global_callback_by_id_callback     { 0x01124C84 };
        inline util::memory_reference<clear_instance_callback_by_name_callback_t> clear_instance_callback_by_name_callback { 0x01124C88 };
        inline util::memory_reference<clear_instance_callback_by_id_callback_t>   clear_instance_callback_by_id_callback   { 0x01124C8C };

        inline util::memory_reference<add_global_callback_callback_t>   add_global_callback_callback   { 0x01124C90 };
        inline util::memory_reference<add_instance_callback_callback_t> add_instance_callback_callback { 0x01124C94 };
        inline util::memory_reference<add_library_callback_callback_t>  add_library_callback_callback  { 0x01124C98 };
    } // references

    // staged by PAE/PARE/ADDR/DPA for the next PSH or POP
    enum e_next_push_pop_modifier : u32 {
        next_push_pop_modifier_none           = 0,
        next_push_pop_modifier_inline_array   = 1,
        next_push_pop_modifier_indirect_array = 2,
        next_push_pop_modifier_address        = 3,
        next_push_pop_modifier_dynamic_array  = 4
    };

    // decoded operand; staged whole into the push/pop modifier argument
    union argument_t {
        u8    raw[0x0C];
        i16   word;          // element width while staged
        f32   number;
        u32   uint;
        void* pointer;

        struct {
            i16 first,
                second;
        } word_pair;
    };

    // individually pooled; a linked chain, not a contiguous stack
    struct vm_thread_flow_stack_element {
        u16*                          pc;       // return address
        script_instance*              instance; // current_instance to restore
        vm_thread_flow_stack_element* previous;
    };

    class vm_thread {

    public:
        script_instance*              inst;
        const script_function*        ex;      // entry function, never changes; derive the running one from `pc`
        vm_thread*                    creator;
        vm_stack                      dstack;
        u16*                          pc;
        e_next_push_pop_modifier      next_push_pop_modifier;
        argument_t                    next_push_pop_modifier_arg;
        f32                           next_push_pop_modifier_subscript;
        vm_thread_flow_stack_element* flow_stack;
        e_opcode                      current_opcode;
        e_opcode_arg                  current_opcode_arg;
        argument_t                    current_arg;
        u32                           current_dsize;
        script_instance*              current_instance;
        u32                           entry;           // native recall state for a BSL that returned false
        void*                         user_data;
        u32                           unk_60;
        f32                           camera_priority; // inherited by BST/BTH children
        u32                           thread_id;
        vm_thread_local_reference*    local_references;
        u32                           unk_70;
        u32                           unk_74;
        vm_simple_list<vm_thread*>*   thread_list;     // the owning instance's `threads`
        vm_thread*                    vm_simple_list_previous;
        vm_thread*                    vm_simple_list_next;
    };

    ASSERT_SIZEOF  (argument_t,                   0x0C);
    ASSERT_SIZEOF  (vm_thread_flow_stack_element, 0x0C);

    ASSERT_SIZEOF  (vm_thread,                                   0x84);
    ASSERT_OFFSETOF(vm_thread, inst,                             0x00);
    ASSERT_OFFSETOF(vm_thread, ex,                               0x04);
    ASSERT_OFFSETOF(vm_thread, creator,                          0x08);
    ASSERT_OFFSETOF(vm_thread, dstack,                           0x0C);
    ASSERT_OFFSETOF(vm_thread, pc,                               0x20);
    ASSERT_OFFSETOF(vm_thread, next_push_pop_modifier,           0x24);
    ASSERT_OFFSETOF(vm_thread, next_push_pop_modifier_arg,       0x28);
    ASSERT_OFFSETOF(vm_thread, next_push_pop_modifier_subscript, 0x34);
    ASSERT_OFFSETOF(vm_thread, flow_stack,                       0x38);
    ASSERT_OFFSETOF(vm_thread, current_opcode,                   0x3C);
    ASSERT_OFFSETOF(vm_thread, current_opcode_arg,               0x40);
    ASSERT_OFFSETOF(vm_thread, current_arg,                      0x44);
    ASSERT_OFFSETOF(vm_thread, current_dsize,                    0x50);
    ASSERT_OFFSETOF(vm_thread, current_instance,                 0x54);
    ASSERT_OFFSETOF(vm_thread, entry,                            0x58);
    ASSERT_OFFSETOF(vm_thread, user_data,                        0x5C);
    ASSERT_OFFSETOF(vm_thread, camera_priority,                  0x64);
    ASSERT_OFFSETOF(vm_thread, thread_id,                        0x68);
    ASSERT_OFFSETOF(vm_thread, local_references,                 0x6C);
    ASSERT_OFFSETOF(vm_thread, thread_list,                      0x78);
    ASSERT_OFFSETOF(vm_thread, vm_simple_list_previous,          0x7C);
    ASSERT_OFFSETOF(vm_thread, vm_simple_list_next,              0x80);
}}} // treyarch::chuck::vm
