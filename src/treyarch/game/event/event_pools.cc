#include "treyarch/game/event/event_pools.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace event_pools {
    memory::fixed_pool &event_pool() {
        static util::memory_reference<memory::fixed_pool> reference { 0x0102C2D8 };

        return reference.get();
    }

    memory::fixed_pool &event_type_pool() {
        static util::memory_reference<memory::fixed_pool> reference { 0x0102C580 };

        return reference.get();
    }

    memory::fixed_pool &recipient_pool() {
        static util::memory_reference<memory::fixed_pool> reference { 0x0102C7A8 };

        return reference.get();
    }
}} // treyarch::event_pools
