#include "treyarch/ngl/list/arena.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

void* ngl::list::allocate(u32 size, u32 alignment) {
    arena_state &state = references::arena.get();

    u32 aligned   = ~(alignment - 1) & ((u32)state.cursor + alignment - 1);
    u32 end       = aligned + size;
    u32 arena_end = (u32)state.base + state.capacity;

    if (end <= arena_end) {
        state.cursor = (u8*)end;
        
        return (void*)aligned;
    }

    memory::report("Render list allocation overflow. Reserved = %d Requested = %d Free = %d.\n",
                   state.capacity,
                   size,
                   arena_end - aligned);

    return nullptr;
}

void ngl::list::replace_storage(u32 capacity) {
    arena_state &state = references::arena.get();

    if (state.base) {
        memory::free(state.base);

        state.base = nullptr;
    }

    if (capacity)
        state.base = (u8*)memory::allocate(capacity, 8, 0);

    state.capacity = capacity;
}

void ngl::list::rewind() {
    arena_state &state = references::arena.get();

    state.cursor = state.base;
}
