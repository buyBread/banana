#include <new>

#include "treyarch/game/environment_progression.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_7EB4B0
environment_progression_state::environment_progression_state() : environment_progression(nullptr),
                                                                 unk_04(0),
                                                                 unk_08(-1),
                                                                 progression_level(0.0f) {}

// sub_428F60
void environment_progression_state::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(environment_progression_state));

    references::environment_progression_state.write(allocation ? new (allocation) environment_progression_state() : nullptr);
}
