#include "game/amalga/resource_handler.hh"
#include "game/amalga/resource_pack_slot.hh"

using namespace treyarch;

// sub_73A4D0
bool amalga::resource_handler_base::handle(e_behavior behavior, limited_timer* time_limit) {
    if (state == progress_done)
        return false;

    resource_directory* directory = pack_slot->pack_directory;

    if (state == progress_start) {
        state             = progress_active;
        descriptor_cursor = directory->type_starts[(u32)type];

        if (behavior == unload)
            directory->type_state[(u32)type] = 0;

        pre_handle_resources(behavior);

        if (time_limit && time_limit->out_of_time())
            return true;
    }

    directory = pack_slot->pack_directory;

    i32 end = directory->type_starts[(u32)type] + directory->type_counts[(u32)type];

    if (descriptor_cursor >= end) {
        state = progress_done;

        return false;
    }

    while (true) {
        i32 result = handle_resource(behavior, pack_slot->pack_directory->descriptors[descriptor_cursor], time_limit);

        if (result == 1)
            return true;

        if (result == 0)
            ++descriptor_cursor;

        if (time_limit && time_limit->out_of_time())
            return true;

        if (descriptor_cursor >= end) {
            state = progress_done;

            return false;
        }
    }
}
