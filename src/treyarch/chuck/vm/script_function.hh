#pragma once

#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_object;
    struct script_function;

    namespace fn {
        inline auto function_retain_argument_references =
            (void(__thiscall*)(script_function*, i32, void*, u32, bool, bool))
            0x00A20440;

        inline auto function_release_argument_references =
            (void(__thiscall*)(script_function*, void*, u32))
            0x00A20560;
    }

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

        void retain_argument_references(void* arguments, u32 size) {
            fn::function_retain_argument_references(this, 0, arguments, size, true, false);
        }

        void release_argument_references(void* arguments, u32 size) {
            fn::function_release_argument_references(this, arguments, size);
        }
    };
}}} // treyarch::chuck
