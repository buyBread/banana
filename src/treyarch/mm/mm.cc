#include "treyarch/mm/mm.hh"
#include "treyarch/mm/superheap.hh"

using namespace treyarch;

// sub_5FBDD0
void* mm::allocate(u32 size) {
    superheap_t* superheap = references::default_space.read()->superheap;

    void* allocation = superheap ? superheap->allocate(size) : nullptr;

    if (allocation)
        return allocation;

    return allocate_storage(size, 0);
}

// sub_8597E0
void mm::free(void* allocation) {
    if (!allocation)
        return;

    if (find_superblock_entry(allocation)) {
        references::default_space.read()->superheap->deallocate(allocation);

        return;
    }

    free_storage(allocation);
}
