#include "treyarch/game/event/chuck_parameter_event.hh"
#include "treyarch/shared/memory/fixed_pool.hh"

namespace treyarch { namespace references {
    util::memory_reference<memory::fixed_pool> chuck_parameter_event_pool { 0x00F22258 };
}} // treyarch::references

using namespace treyarch;

// inlined @ sub_843140, sub_843220
chuck_parameter_event::chuck_parameter_event(string_hash signal) : args_stack_size(0) {
    event_type_id = signal;
}

// sub_719A10
chuck_parameter_event::~chuck_parameter_event() = default;

// sub_825850
void* chuck_parameter_event::operator new(std::size_t) noexcept {
    return references::chuck_parameter_event_pool.get().allocate();
}

// sub_7199D0
void chuck_parameter_event::operator delete(void* allocation) noexcept {
    references::chuck_parameter_event_pool.get().release(allocation);
}

// sub_843120
void chuck_parameter_event::construct_mashed_class() {
    event::construct_mashed_class();
}

// sub_719A70
mash::virtual_types_key chuck_parameter_event::get_virtual_type_key() const {
    return references::chuck_parameter_event_type_key.read();
}

// sub_719950
bool chuck_parameter_event::is_subclass_of(mash::virtual_types_key parent_class) const {
    return parent_class == event::get_virtual_type_key() || event::is_subclass_of(parent_class);
}

// sub_4D57E0
i32 chuck_parameter_event::get_mash_sizeof() const {
    return sizeof(*this);
}
