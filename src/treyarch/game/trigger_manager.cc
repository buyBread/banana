#include <new>

#include "treyarch/game/trigger_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_738F40
trigger_manager::trigger_manager() {
    unk_0000[0] = 0;
    unk_0000[1] = 0;
    unk_0008    = 0;
    unk_0009    = 0;
    unk_000c[0] = 0;
    unk_000c[1] = 0;

    for (u32 &value : unk_0014)
        value = 0;

    for (u32 index = 0; index < 1024; ++index) {
        unk_0040[index][0] = 0;
        unk_0040[index][1] = 0;
    }

    unk_2040 = 0;
}

// sub_428A10
void trigger_manager::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(trigger_manager));

    references::trigger_manager.write(allocation ? new (allocation) trigger_manager() : nullptr);

    // retail ends with an empty call (nullsub_1)
}
