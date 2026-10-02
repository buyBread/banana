#pragma once

#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class vm_thread;

    /* buffers come from pools of 128, 284, 512 and 1024 bytes; growth steps through them and preserves `sp - buffer`.
       push stays correct when its source points into this stack. */
    class vm_stack {

    public:
        u8*        sp;           // next free byte
        void*      stack_buffer; // pool allocation handed back on release
        u8*        buffer;       // moves when the stack grows
        u32        buffer_size;
        vm_thread* thread;

        vm_stack(vm_thread* owner, u32 size);

        void allocate(u32 size);
        void grow();
        void push(const void* source, i32 size);
    };

    namespace references {
        inline util::memory_reference<memory::fixed_pool> stack_pool_128  { 0x01124898 };
        inline util::memory_reference<memory::fixed_pool> stack_pool_284  { 0x01124850 };
        inline util::memory_reference<memory::fixed_pool> stack_pool_512  { 0x01124808 };
        inline util::memory_reference<memory::fixed_pool> stack_pool_1024 { 0x011247C0 };

        // push copies its source here first whenever the stack has to grow
        inline util::memory_reference<void*> push_staging_buffer { 0x011247B0 };

        inline util::memory_reference<u32> push_staging_size { 0x011247B4 };

        inline util::memory_reference<engine_recursive_lock*> push_staging_lock { 0x011247B8 };
    } // references

    ASSERT_SIZEOF  (vm_stack,               0x14);
    ASSERT_OFFSETOF(vm_stack, sp,           0x00);
    ASSERT_OFFSETOF(vm_stack, stack_buffer, 0x04);
    ASSERT_OFFSETOF(vm_stack, buffer,       0x08);
    ASSERT_OFFSETOF(vm_stack, buffer_size,  0x0C);
    ASSERT_OFFSETOF(vm_stack, thread,       0x10);
}}} // treyarch::chuck::vm
