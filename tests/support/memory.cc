#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <new>
#include <doctest/doctest.h>

#include "support/memory.hh"
#include "treyarch/mm/heap.hh"
#include "treyarch/base/memory.hh"

tests::c_memory_fixture* tests::current_memory_fixture = nullptr;

tests::c_memory_fixture::c_memory_fixture() : m_previous(current_memory_fixture) {
    current_memory_fixture = this;
}

tests::c_memory_fixture::~c_memory_fixture() {
    CHECK_MESSAGE(invalid_frees == 0, "unexpected pointer or allocator passed to free");

    // fixture cleanup keeps one test's leftover allocations from accumulating in the runner.
    // it does not count as a free made by the engine code.
    for (const allocation_t &allocation : allocations) {
        if (!allocation.released)
            _aligned_free(allocation.pointer);
    }

    current_memory_fixture = m_previous;
}

void* tests::c_memory_fixture::allocate(e_allocator allocator, u32 size, u32 alignment, u32 flags) {
    // give even a zero-byte request usable storage, but record the original request below.
    void* pointer = _aligned_malloc(std::max(size, 1u), std::max(alignment, (u32)sizeof(void*)));

    if (!pointer)
        throw std::bad_alloc();

    std::memset(pointer, 0xCD, std::max(size, 1u));
    allocations.push_back({pointer, size, alignment, flags, allocator, false, {}});

    return pointer;
}

void tests::c_memory_fixture::free(e_allocator allocator, void* pointer) {
    frees.push_back({pointer, allocator});

    if (!pointer)
        return;

    // an address can be reused after a free, so the newest live allocation gets the first look.
    for (auto allocation = allocations.rbegin(); allocation != allocations.rend(); ++allocation) {
        if (allocation->pointer != pointer || allocation->released)
            continue;

        if (allocation->allocator != allocator) {
            // report a bad free at fixture teardown without handing it to the host allocator.
            ++invalid_frees;

            return;
        }

        allocation->released_bytes.resize(allocation->size);
        
        if (allocation->size)
            std::memcpy(allocation->released_bytes.data(), pointer, allocation->size);
        
        allocation->released = true;
        _aligned_free(pointer);
        
        return;
    }

    ++invalid_frees;
}

const tests::allocation_t* tests::c_memory_fixture::find(const void* pointer) const {
    for (auto allocation = allocations.rbegin(); allocation != allocations.rend(); ++allocation) {
        if (allocation->pointer == pointer)
            return &*allocation;
    }

    return nullptr;
}

bool tests::c_memory_fixture::contains(const void* pointer) const {
    // array elements live inside a larger allocation rather than at its starting address.
    for (const allocation_t &allocation : allocations) {
        if (!allocation.released &&
            (size_t)pointer >= (size_t)allocation.pointer &&
            (size_t)pointer <  (size_t)allocation.pointer + allocation.size)
            
            return true;
    }

    return false;
}

u32 tests::c_memory_fixture::allocation_count(e_allocator allocator) const {
    u32 count = 0;

    for (const allocation_t &allocation : allocations)
        count += allocation.allocator == allocator;

    return count;
}

u32 tests::c_memory_fixture::free_count(e_allocator allocator) const {
    u32 count = 0;

    for (const free_call_t &call : frees)
        count += call.pointer && call.allocator == allocator;

    return count;
}

u32 tests::c_memory_fixture::live_count(e_allocator allocator) const {
    u32 count = 0;
    for (const allocation_t &allocation : allocations)
        count += allocation.allocator == allocator && !allocation.released;
    return count;
}

// the test executable links these allocator entry points in place of the game's allocators.
// mash code still makes its usual calls; the active fixture supplies and tracks the memory.
void* treyarch::memory::heap::allocate(u32 size) {
    return tests::current_memory_fixture
        ->allocate(tests::heap_allocator, size);
}

void treyarch::memory::heap::free(void* allocation) {
    tests::current_memory_fixture
        ->free(tests::heap_allocator, allocation);
}

void* treyarch::memory::allocate(u32 size, u32 alignment, u32 flags) {
    return tests::current_memory_fixture
        ->allocate(tests::plain_allocator, size, alignment, flags);
}

void treyarch::memory::free(void* allocation) {
    tests::current_memory_fixture
        ->free(tests::plain_allocator, allocation);
}
