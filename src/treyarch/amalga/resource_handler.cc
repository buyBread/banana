#include "treyarch/amalga/merged_apk/resource_handler.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/amalga/script/resource_handler.hh"

using namespace treyarch;

// sub_73A4D0
bool amalga::resource_handler::advance(i32 operation, limited_timer* time_limit) {
    if (state == 2)
        return false;

    resource_directory* directory = pack_slot->pack_directory;

    if (state == 0) {
        state             = 1;
        descriptor_cursor = directory->type_starts[(u32)type];

        if (operation == 1)
            directory->type_state[(u32)type] = 0;

        switch(type) {
            case e_resource_type::merged_apk:
                ((merged_apk_resource_handler*)this)->begin_merged_apk(operation);

                break;

            case e_resource_type::script:
            case e_resource_type::script_gv:
            case e_resource_type::script_sv:
                break;

            default:
                begin(operation);

                break;
        }

        if (time_limit && time_limit->out_of_time())
            return true;
    }

    directory = pack_slot->pack_directory;

    i32 end = directory->type_starts[(u32)type] + directory->type_counts[(u32)type];

    if (descriptor_cursor >= end) {
        state = 2;

        return false;
    }

    while (true) {
        resource_descriptor* descriptor = pack_slot->pack_directory->descriptors[descriptor_cursor];

        i32 result;

        switch(type) {
            case e_resource_type::merged_apk:
                result = ((merged_apk_resource_handler*)this)->progress_merged_apk(operation, descriptor);

                break;

            case e_resource_type::script:
                result = ((script_resource_handler*)this)->progress_mashable(operation, descriptor);

                break;

            case e_resource_type::script_gv:
            case e_resource_type::script_sv:
                result = ((script_var_container_resource_handler*)this)->progress_mashable(operation, descriptor);

                break;

            default:
                result = progress(operation, descriptor, time_limit);

                break;
        }

        if (result == 1)
            return true;

        if (result == 0)
            ++descriptor_cursor;

        if (time_limit && time_limit->out_of_time())
            return true;

        if (descriptor_cursor >= end) {
            state = 2;

            return false;
        }
    }
}
