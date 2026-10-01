#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_object;

    enum e_script_function_flags : u16 {
        script_function_flag_static                  = 0x01,
        script_function_flag_from_mash               = 0x02,
        script_function_flag_locals                  = 0x04,
        script_function_flag_small_stack_recommended = 0x08, // 128-byte thread stack
        script_function_flag_large_stack_recommended = 0x10, // 512-byte thread stack
        script_function_flag_parms_builder           = 0x20
    };

    // how a reference-bearing argument is retained (sub_A20440) and released (sub_A20560)
    enum e_vm_reference_kind : u16 {
        vm_reference_kind_dynamic_array                 = 0,
        vm_reference_kind_string                        = 1,
        vm_reference_kind_ignored                       = 2,
        vm_reference_kind_string_dynamic_array          = 3,
        vm_reference_kind_script_instance_dynamic_array = 4
    };

    struct vm_reference_descriptor {
        e_vm_reference_kind kind;
        u16                 offset; // into the argument block
    };

    ASSERT_SIZEOF(vm_reference_descriptor, 0x04);

    class script_function {

    public:
        // retail addition; the container's mash_image_offset is what separates inline from allocated records
        mash::vector_basic<vm_reference_descriptor> reference_descriptors;

        string_hash             fullname;        // typed signature; what CGC/CIC compare a name against
        string_hash             name;            // without the parameter list
        string_hash             event_parms_key;
        script_object*          parent;
        u16*                    buffer;          // linked wordcode
        u16                     parms_stacksize; // argument bytes
        u16                     event_parms_stacksize;
        u16                     buffer_len;
        e_script_function_flags flags;
    };

    ASSERT_SIZEOF  (script_function,                        0x2C);
    ASSERT_OFFSETOF(script_function, reference_descriptors, 0x00);
    ASSERT_OFFSETOF(script_function, fullname,              0x10);
    ASSERT_OFFSETOF(script_function, name,                  0x14);
    ASSERT_OFFSETOF(script_function, event_parms_key,       0x18);
    ASSERT_OFFSETOF(script_function, parent,                0x1C);
    ASSERT_OFFSETOF(script_function, buffer,                0x20);
    ASSERT_OFFSETOF(script_function, parms_stacksize,       0x24);
    ASSERT_OFFSETOF(script_function, event_parms_stacksize, 0x26);
    ASSERT_OFFSETOF(script_function, buffer_len,            0x28);
    ASSERT_OFFSETOF(script_function, flags,                 0x2A);
}}} // treyarch::chuck::vm
