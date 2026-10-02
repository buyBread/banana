#include <cstring>

#include "treyarch/chuck/vm/vm_stack.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A19F20
vm_stack::vm_stack(vm_thread* owner, u32 size) : sp(nullptr),
                                                 stack_buffer(nullptr),
                                                 buffer(nullptr),
                                                 buffer_size(0),
                                                 thread(owner) {

    allocate(size);
}

// sub_A19A30
void vm_stack::allocate(u32 size) {
    if (!size)
        size = 284;

    buffer_size = size;

    memory::fixed_pool* pool;

    switch (size) {
        case 128:  pool = &references::stack_pool_128.get();  break;
        case 284:  pool = &references::stack_pool_284.get();  break;
        case 512:  pool = &references::stack_pool_512.get();  break;
        case 1024: pool = &references::stack_pool_1024.get(); break;

        // other sizes get no buffer
        default:
            sp = buffer;

            return;
    }

    stack_buffer = pool->allocate();
    buffer       = (u8*)stack_buffer;
    sp           = buffer;
}

// sub_A19AC0
void vm_stack::grow() {
    memory::fixed_pool* source_pool;
    memory::fixed_pool* target_pool;
    u32                 target_size;

    switch (buffer_size) {
        case 128:
            source_pool = &references::stack_pool_128.get();
            target_pool = &references::stack_pool_284.get();
            target_size = 284;
            break;

        case 284:
            source_pool = &references::stack_pool_284.get();
            target_pool = &references::stack_pool_512.get();
            target_size = 512;
            break;

        case 512:
            source_pool = &references::stack_pool_512.get();
            target_pool = &references::stack_pool_1024.get();
            target_size = 1024;
            break;

        // a 1024-byte stack cannot grow
        default:
            return;
    }

    i32   used       = (i32)(sp - buffer);
    void* allocation = target_pool->allocate();

    std::memcpy(allocation, buffer, buffer_size);

    void* released = stack_buffer;

    buffer      = (u8*)allocation;
    buffer_size = target_size;

    if (released)
        source_pool->release(released);

    sp           = buffer + used;
    stack_buffer = allocation;
}

// sub_4E4F70
void vm_stack::push(const void* source, i32 size) {
    const void* copied = source;

    if (size + (i32)(sp - buffer) > (i32)buffer_size) {
        references::push_staging_lock.read()->acquire();

        if ((u32)size > references::push_staging_size.read()) {
            if (references::push_staging_buffer.read())
                memory::heap::free(references::push_staging_buffer.read());

            references::push_staging_size.write(size <= 12 ? 12 : size);
            references::push_staging_buffer.write(memory::heap::allocate(references::push_staging_size.read()));
        }

        std::memcpy(references::push_staging_buffer.read(), source, size);
        copied = references::push_staging_buffer.read();

        // grows one step only
        if (size + (i32)(sp - buffer) > (i32)buffer_size)
            grow();
    }

    std::memcpy(sp, copied, size);
    sp += size;

    if (copied == references::push_staging_buffer.read())
        references::push_staging_lock.read()->release();
}
