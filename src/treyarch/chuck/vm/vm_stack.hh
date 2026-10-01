#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class vm_thread;

    /* growth (sub_A19AC0) steps through pooled 128/284/512/1024-byte buffers and preserves `sp - buffer`.
       sub_4E4F70 is the generic byte push; it stays correct when its source points into this stack. */
    class vm_stack {

    public:
        u8*        sp;           // next free byte
        void*      stack_buffer; // pool allocation handed back on release
        u8*        buffer;       // moves when the stack grows
        u32        buffer_size;
        vm_thread* thread;
    };

    ASSERT_SIZEOF  (vm_stack,               0x14);
    ASSERT_OFFSETOF(vm_stack, sp,           0x00);
    ASSERT_OFFSETOF(vm_stack, stack_buffer, 0x04);
    ASSERT_OFFSETOF(vm_stack, buffer,       0x08);
    ASSERT_OFFSETOF(vm_stack, buffer_size,  0x0C);
    ASSERT_OFFSETOF(vm_stack, thread,       0x10);
}}} // treyarch::chuck::vm
