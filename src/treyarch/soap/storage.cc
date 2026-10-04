#include "retail.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/storage.hh"

using namespace treyarch;

// sub_9EDA50
soap::storage* soap::storage::inst() {
    if (!references::storage.read()) {
        void* allocation = memory::heap::allocate(sizeof(storage));

        references::storage.write(allocation ? (storage*)retail::sub_9F0950((i32)allocation) : nullptr);
    }

    return references::storage.read();
}
