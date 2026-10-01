#pragma once

#include "treyarch/chuck/vm/vm_stack.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    class script_library_function;

    // returning false makes BSL (sub_A21D60) restore pc and the stack offset, mark the thread for recall, and retry next run
    using script_library_function_invoke = bool (__thiscall*)(const script_library_function* self, vm::vm_stack &stack);

    struct script_library_function_vtable {
        script_library_function_invoke invoke; // base returns true (sub_823B70)
    };

    /* stateless native descriptor.
       its position is the BSL operand: global functions append to class 0 (sub_A21650), the rest to their own class (sub_A215E0).
       the owning class frees it with the engine heap. */
    class script_library_function {

    public:
        script_library_function_vtable* vtable;
    };

    ASSERT_SIZEOF(script_library_function_vtable, 0x04);
    ASSERT_SIZEOF(script_library_function,        0x04);
}}} // treyarch::chuck::script_library
