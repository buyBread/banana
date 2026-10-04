#include "retail.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/notification_manager.hh"

using namespace treyarch;

// sub_9EF540
soap::notification_manager* soap::notification_manager::inst() {
    if (!references::notification_manager.read()) {
        void* allocation = memory::heap::allocate(sizeof(notification_manager));

        references::notification_manager.write(allocation ? (notification_manager*)retail::sub_9EF2A0((u32*)allocation) : nullptr);
    }

    return references::notification_manager.read();
}
