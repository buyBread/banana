#pragma once

#include <cstddef>

#include "treyarch/chuck/vm/vm_stack.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    class script_library_class;

    /* stateless native descriptor.
       its position is the BSL operand: global functions append to slc_global, the rest to their own class.
       the owning class frees it with the engine heap. */
    class script_library_function {

    public:
        // the vtable slot
        using invoke_t = bool (__thiscall*)(script_library_function* self, vm::vm_stack &stack);

        explicit script_library_function(script_library_class* slc);
        script_library_function(); // global

        // returning false makes BSL (sub_A21D60) restore pc and the stack offset, mark the thread for recall, and retry next run
        virtual bool operator()(vm::vm_stack &stack);

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    ASSERT_SIZEOF(script_library_function, 0x04);
}}} // treyarch::chuck::script_library
