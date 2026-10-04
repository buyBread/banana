#include "retail.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/online.hh"

using namespace treyarch;

// sub_9ED060
soap::online* soap::online::inst() {
    if (!references::online.read()) {
        void*   allocation = memory::heap::allocate(sizeof(online));
        online* instance   = nullptr;

        // online_pc's constructor, inlined
        if (allocation) {
            instance         = (online*)retail::sub_9ECFB0((u32*)allocation);
            instance->vtable = &references::online_pc_vtable.get();
        }

        references::online.write(instance);
    }

    return references::online.read();
}
