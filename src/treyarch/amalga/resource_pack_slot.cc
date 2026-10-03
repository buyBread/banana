#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/nfl/nfl.hh"

using namespace treyarch;

// sub_76A850
void amalga::resource_pack_slot::frame_advance(f32, limited_timer* time_limit) {
    switch (slot_state) {
        case slot_state_streaming:
            // the result goes nowhere
            nfl::media_status();

            return;

        case slot_state_constructing: {
            push_resource_context_stack_object context(this);

            if (!vtable->on_load(this, time_limit)) {
                slot_state = slot_state_ready;

                if (my_callback)
                    my_callback(callback_construct, &my_partition->streamer, this, time_limit);
            }

            break;
        }

        case slot_state_destructing: {
            push_resource_context_stack_object context(this);

            // a callback that returns true holds the unload off for now
            const bool waiting = my_callback ?
                my_callback(callback_destruct, &my_partition->streamer, this, time_limit) : false;

            if (!waiting && !vtable->on_unload(this, time_limit))
                slot_state = slot_state_empty;

            break;
        }
    }
}

// sub_73A0C0
void amalga::resource_pack_slot::notify_load_started(resource_amalgatoc_pack_entry* entry,
                                                     resource_pack_slot_callback    callback) {

    vtable->clear_pack(this);

    pack_name_str = entry->name;
    unk_30        = entry->unk_40;
    pack_name     = resource_key { entry->name_hash, e_resource_type::packfile };
    my_callback   = callback;
    slot_state    = slot_state_streaming;

    if (callback)
        callback(callback_load_started, &my_partition->streamer, this, nullptr);
}
