#include <windows.h>
#include <dbghelp.h>
#include <cstdlib>
#include <cstring>
#include <new>

#include "treyarch/mm/dlmalloc.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/mm/superheap.hh"

using namespace treyarch;

namespace treyarch { namespace mm { namespace references {
    util::memory_reference<u8> default_space_storage { 0x00FFD8D8 };
}}} // treyarch::mm::references

// sub_5F8870
void mm::memory_space::report_usage(bool force) {
    // a space reports again once its size or use moves by its threshold
    for (memory_space* space = references::list_head.read(); space; space = space->list_next) {
        if (force                                                                              ||
            (u32)std::abs((i32)(space->size - space->reported_size)) > space->report_threshold ||
            (u32)std::abs((i32)(space->used - space->reported_used)) > space->report_threshold) {

            space->reported_used = space->used;
            space->reported_size = space->size;
        }
    }
}

// sub_5F88C0
void mm::memory_space::allocation_failure(malloc_state* failed_space) {
    for (memory_space* next = references::list_head.read(); next; next = next->list_next) {
        next->reported_used = next->used;
        next->reported_size = next->size;
    }

    memory_space* space = references::list_head.read();

    while (space && space->space != failed_space)
        space = space->list_next;

    superheap_t::dump();

    if (space && space->superheap) {
        u32 super_used;
        u32 super_size;

        space->superheap->get_usage(&super_used, &super_size);
    }
}

// sub_5FABD0
mm::memory_space::~memory_space() {
    if (space)
        destroy_mspace(space);

    space = nullptr;
}

// sub_5FABF0
u32 mm::memory_space::allocation_size_general(void* allocation) {
    lock();

    u32 bytes = mspace_usable_size(allocation);

    unlock();

    return bytes;
}

// sub_5FAF00
void* mm::memory_space::allocate_general(u32 bytes, u32 alignment) {
    lock();

    if (alignment < 16)
        alignment = 16;

    void* allocation = mspace_memalign(space, alignment, bytes);

    used += mspace_usable_size(allocation);

    unlock();

    return allocation;
}

// sub_5FAFB0
void mm::memory_space::deallocate_general(void* allocation) {
    used -= allocation_size_general(allocation);

    lock();
    mspace_free(space, allocation);
    unlock();
}

// sub_5FB090
u32 mm::memory_space::allocation_size(void* allocation) {
    if (!allocation)
        return 0;

    superblock_entry_t* entry = superheap ? find_superblock_entry(allocation) : nullptr;

    if (entry)
        return entry->block_size;

    return allocation_size_general(allocation);
}

// sub_5FB270
mm::memory_space::memory_space(const char* space_name, e_space_type space_type, u32 hints, u32 capacity, void* base) {
    list_next = references::list_head.read();
    references::list_head.write(this);

    std::strncpy(name, space_name, sizeof(name));
    name[31] = 0;

    space            = nullptr;
    type             = space_type;
    hint_flags       = hints;
    initial_size     = capacity;
    max_size         = 0;
    reserved_030     = 0;
    spin_lock        = 0;
    used             = 0;
    size             = 0;
    reported_used    = 0;
    reported_size    = 0;
    report_threshold = 0x100000;

    memory_callbacks callbacks { this, map_memory, unmap_memory };

    // physical, virtual and vram spaces map their own memory; a fixed one sits in its base when it has one
    if (type <= type_vram)
        space = create_mspace(initial_size, 0, &callbacks);
    else if (type == type_fixed) {
        max_size = initial_size;

        if (base)
            space = create_mspace_with_base(base, initial_size, 0, &callbacks);
        else
            space = create_mspace(initial_size, 0, &callbacks);
    }

    if (!(hint_flags & hint_superheap)) {
        superheap = nullptr;

        return;
    }

    void* storage = get_default_space()->allocate_general(sizeof(superheap_t), 0);

    superheap = storage ? new (storage) superheap_t() : nullptr;
    superheap->initialize(this);
}

// sub_5FB3D0
void mm::init() {
    if (references::default_space.read())
        return;

    // symbols for the allocation stack traces
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);

    memory_space* space = (memory_space*)&references::default_space_storage.get();

    references::default_space.write(space);
    new (space) memory_space("default", type_virtual, hint_superheap, 0x01000000, nullptr);

    load_module_information();

    for (memory_space* next = references::list_head.read(); next; next = next->list_next) {
        next->reported_used = next->used;
        next->reported_size = next->size;
    }
}

// inlined @ sub_5FBDD0
// inlined @ sub_8597E0
// inlined @ sub_5FB270
// inlined @ sub_5FB470
// inlined @ sub_5FB4F0
// inlined @ sub_5FB7D0
// inlined @ sub_5FB7A0
// inlined @ sub_5FBE30
// inlined @ sub_5FBFD0
// inlined @ sub_76E400
// inlined @ sub_770160
mm::memory_space* mm::get_default_space() {
    if (!references::default_space.read())
        init();

    return references::default_space.read();
}

// sub_5FB7A0
void* mm::allocate_general(u32 size, u32 alignment) {
    return get_default_space()->allocate_general(size, alignment);
}

// sub_5FBD20
void* mm::memory_space::allocate(u32 bytes, u32 alignment) {
    if (superheap) {
        // small blocks come from the superheap when a size class fits
        u32 block_size = superheap->get_block_size(bytes, alignment);

        if (block_size) {
            void* allocation = superheap->allocate(bytes);

            used += block_size;

            if (allocation)
                return allocation;
        }
    }

    return allocate_general(bytes, alignment);
}

// sub_5FBD70
void mm::memory_space::deallocate(void* allocation) {
    if (!allocation)
        return;

    superblock_entry_t* entry = superheap ? find_superblock_entry(allocation) : nullptr;

    if (entry) {
        used -= entry->block_size;

        superheap->deallocate(allocation, entry);
    } else
        deallocate_general(allocation);
}

// sub_5FBDD0
void* mm::allocate(u32 size) {
    return get_default_space()->allocate(size, 0);
}

// sub_5FBE30
void* mm::allocate(u32 size, u32 alignment) {
    return get_default_space()->allocate(size, alignment);
}

// sub_5FBE90
void* mm::memory_space::reallocate_general(void* allocation, u32 bytes, u32 alignment) {
    if (alignment < 16)
        alignment = 16;

    lock();

    void* moved = mspace_memalign(space, alignment, bytes);

    unlock();

    used += allocation_size_general(moved);

    u32 copied = allocation_size(allocation);

    if (copied > bytes)
        copied = bytes;

    std::memcpy(moved, allocation, copied);
    deallocate(allocation);

    return moved;
}

// sub_5FBF50
void* mm::memory_space::reallocate(void* allocation, u32 bytes, u32 alignment) {
    u32 block_size = superheap ? superheap->get_block_size(bytes, alignment) : 0;

    if (!block_size)
        return reallocate_general(allocation, bytes, alignment);

    // a small block moves into the superheap
    void* moved = superheap->allocate(bytes);

    used += block_size;

    u32 copied = allocation_size(allocation);

    if (copied > bytes)
        copied = bytes;

    std::memcpy(moved, allocation, copied);
    deallocate(allocation);

    return moved;
}

// sub_5FBFD0
void* mm::reallocate(void* allocation, u32 size, u32 alignment) {
    return get_default_space()->reallocate(allocation, size, alignment);
}

// sub_8597E0
void mm::free(void* allocation) {
    get_default_space()->deallocate(allocation);
}
