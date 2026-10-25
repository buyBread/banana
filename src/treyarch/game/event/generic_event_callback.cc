#include "treyarch/game/arch_base.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/event/generic_event_callback.hh"

using namespace treyarch;

// sub_683CA0
generic_event_callback::generic_event_callback() : callback_id(0),
                                                   times_raised(0) {}

// sub_683CB0
void generic_event_callback::construct_mashed_class() {
    callback_id  = 0;
    times_raised = 0;
}

// sub_683930
void generic_event_callback::add(arch_base_vhandle owner, string_hash event_type_id, code_event_callback_function function, void* parameters) {
    if (arch_base* base = owner.resolve()) {
        if (function == count_raise)
            parameters = &times_raised;

        callback_id = base->add_callback(event_type_id, function, parameters, false);
    }

    times_raised = 0;
}

// sub_684EF0
void generic_event_callback::remove(arch_base_vhandle owner, string_hash event_type_id) {
    if (!callback_id)
        return;

    if (arch_base* base = owner.resolve())
        base->remove_callback(callback_id, event_type_id);
    else
        event_manager::remove_callback(callback_id, event_type_id, owner);

    callback_id = 0;
}

// sub_44E670
void generic_event_callback::count_raise(event*, arch_base_vhandle, void* parameters) {
    ++*(u32*)parameters;
}
