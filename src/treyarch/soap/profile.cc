#include "retail.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/profile.hh"

using namespace treyarch;

// sub_9ED670
soap::profile* soap::profile::inst() {
    if (!references::profile.read()) {
        void* allocation = memory::heap::allocate(sizeof(profile));

        references::profile.write(allocation ? (profile*)retail::sub_9EFA50((u32*)allocation) : nullptr);
    }

    return references::profile.read();
}
