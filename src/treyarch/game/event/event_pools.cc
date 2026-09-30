#include "treyarch/game/event/event_pools.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace event_pools { namespace references {
    util::memory_reference<memory::fixed_pool> event_pool      { 0x0102C2D8 };
    util::memory_reference<memory::fixed_pool> event_type_pool { 0x0102C580 };
    util::memory_reference<memory::fixed_pool> recipient_pool  { 0x0102C7A8 };
}}} // treyarch::event_pools::references

using namespace treyarch;

memory::fixed_pool &event_pools::event_pool() {
    return references::event_pool.get();
}

memory::fixed_pool &event_pools::event_type_pool() {
    return references::event_type_pool.get();
}

memory::fixed_pool &event_pools::recipient_pool() {
    return references::recipient_pool.get();
}
