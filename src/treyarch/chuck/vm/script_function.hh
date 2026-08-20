#pragma once

#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_object;

    enum class vm_reference_kind : u16 {
        dynamic_array                 = 0,
        string                        = 1,
        ignored                       = 2,
        string_dynamic_array          = 3,
        script_instance_dynamic_array = 4
    };

    struct vm_reference_descriptor {
        vm_reference_kind kind;
        u16               offset;
    };

    struct script_function {
        u32                      record_size; // allocation extent used to distinguish inline from separately allocated descriptor storage
        u32                      reference_descriptor_count;
        vm_reference_descriptor* reference_descriptors;
        u32                      unk_0c;
        u32                      signature_hash; // includes the qualified typed parameter list
        u32                      function_hash;  // omits it
        u32                      optional_string_hash;
        script_object*           object;
        u8*                      code;
        u32                      stack_metadata; // 16 low bits hold the argument byte size
        u16                      unk_28;
        u16                      flags;

        u16 argument_size() const noexcept {
            return (u16)stack_metadata;
        }
    };
}}} // treyarch::chuck