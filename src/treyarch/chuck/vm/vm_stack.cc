#include <cstring>

#include "treyarch/chuck/vm/vm_stack.hh"
#include "treyarch/mm/heap.hh"

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

// sub_A19F50
vm_stack::~vm_stack() {
    if (!stack_buffer)
        return;

    switch (buffer_size) {
        case 128:  delete (vm_stack_buffer_128*) stack_buffer; break;
        case 284:  delete (vm_stack_buffer_284*) stack_buffer; break;
        case 512:  delete (vm_stack_buffer_512*) stack_buffer; break;
        case 1024: delete (vm_stack_buffer_1024*)stack_buffer; break;
        default:   break;
    }
}

// sub_A196D0
void vm_stack::create_push_staging_lock() {
    auto* lock = (engine_recursive_lock*)memory::heap::allocate(sizeof(engine_recursive_lock));

    if (lock) {
        lock->owner = 0;
        lock->state = 0;
        lock->depth = 0;
    }

    references::push_staging_lock.write(lock);

    lock->owner = 0;
    lock->state = 0;
    lock->depth = 0;
}

// sub_A19A30
void vm_stack::allocate(u32 size) {
    if (!size)
        size = 284;

    buffer_size = size;

    switch (size) {
        case 128:  stack_buffer = new vm_stack_buffer_128;  break;
        case 284:  stack_buffer = new vm_stack_buffer_284;  break;
        case 512:  stack_buffer = new vm_stack_buffer_512;  break;
        case 1024: stack_buffer = new vm_stack_buffer_1024; break;

        // other sizes get no buffer
        default:
            sp = buffer;

            return;
    }

    buffer = (u8*)stack_buffer;
    sp     = buffer;
}

// inlined @ sub_A19AC0
template<typename from_t, typename to_t>
void vm_stack::grow_buffer(u32 target_size) {
    i32   used       = (i32)(sp - buffer);
    to_t* allocation = new to_t;

    std::memcpy(allocation, buffer, buffer_size);

    from_t* released = (from_t*)stack_buffer;

    buffer      = (u8*)allocation;
    buffer_size = target_size;

    if (released)
        delete released;

    sp           = buffer + used;
    stack_buffer = allocation;
}

// sub_A19AC0
void vm_stack::grow() {
    switch (buffer_size) {
        case 128: grow_buffer<vm_stack_buffer_128, vm_stack_buffer_284> (284);  break;
        case 284: grow_buffer<vm_stack_buffer_284, vm_stack_buffer_512> (512);  break;
        case 512: grow_buffer<vm_stack_buffer_512, vm_stack_buffer_1024>(1024); break;

        // a 1024-byte stack cannot grow
        default:
            break;
    }
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

// sub_A1C750
void vm_stack::push_num(f32 value) {
    if ((i32)(sp - buffer) + 4 > (i32)buffer_size)
        grow();

    *(f32*)sp = value;
    sp += 4;
}

// sub_A21EB0
void vm_stack::push_uint(u32 value) {
    if ((i32)(sp - buffer) + 4 > (i32)buffer_size)
        grow();

    *(u32*)sp = value;
    sp += 4;
}

// sub_8435B0
void vm_stack::pop(i32 size) {
    if (size <= 0 && (i32)(sp - buffer) - size > (i32)buffer_size)
        grow();

    sp -= size;
}

// sub_83D8F0
void vm_stack::move_sp(i32 size) {
    if (size >= 0 && size + (i32)(sp - buffer) > (i32)buffer_size)
        grow();

    sp += size;
}

// sub_A22100
void vm_stack::move_sp_and_zero(i32 size) {
    i32 used = (i32)(sp - buffer);

    if (size >= 0 && used + size > (i32)buffer_size)
        grow();

    sp += size;

    // a negative size reaches memset as is
    std::memset(buffer + used, 0, size);
}

// sub_A220F0
string_hash vm_stack::pop_signal() {
    sp -= 4;

    return string_hash(*(u32*)sp);
}

// sub_A1A010
void vm_stack::initialize_pools() {
    references::stack_pool_128 .get().initialize("vm_stack_buffer", 0x80,  4, 0x40, 4);
    references::stack_pool_284 .get().initialize("vm_stack_buffer", 0x11C, 4, 0x20, 1);
    references::stack_pool_512 .get().initialize("vm_stack_buffer", 0x200, 4, 0x20, 2);
    references::stack_pool_1024.get().initialize("vm_stack_buffer", 0x400, 4, 0x02, 1);
}
