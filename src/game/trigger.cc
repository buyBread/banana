#include <new>

#include "game/trigger.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_72CD60
void trigger::set_uses_extended_events(bool on) {
    if (on)
        flags |= trigger_flag_extended_events;
    else
        flags &= ~trigger_flag_extended_events;
}

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
    void* allocation = mm::allocate(sizeof(trigger_manager));

    trigger_manager::instance() = allocation ? new (allocation) trigger_manager() : nullptr;

    // retail ends with an empty call (nullsub_1)
}
