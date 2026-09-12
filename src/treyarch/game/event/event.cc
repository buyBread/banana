#include "treyarch/game/event/event.hh"
#include "treyarch/game/event/event_pools.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

event::event(string_hash requested_event_type_id, bool requested_autokill) : event_type_id(requested_event_type_id),
                                                                             raised_frame(0),
                                                                             autokill(requested_autokill),
                                                                             from_mash(false),
                                                                             padding_0e {} {}

void* event::operator new(std::size_t) {
    return event_pools::event_pool().allocate();
}

void event::operator delete(void* allocation) noexcept {
    event_pools::event_pool().release(allocation);
}

void event::construct_mashed_class() {
    event_type_id = string_hash();
    from_mash     = true;
}

mash::virtual_types_key event::get_virtual_type_key() const {
    static util::memory_reference<mash::virtual_types_key> event_type_key { 0x0102C410 };

    return event_type_key.read();
}

bool event::is_subclass_of(mash::virtual_types_key parent_class) const {
    return parent_class == mash::mash_virtual_base::get_virtual_type_key();
}

void event::raise() {
    static util::memory_reference<u8*> game_state { 0x010FC54C };

    raised_frame = *(u32*)(game_state.read() + 0xF0);
}
