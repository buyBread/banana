#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/wds/ai/core_ai_resource.hh"

using namespace treyarch;

// sub_4CE5F0
void core_ai_resource::construct_mashed_class() {
    self = nullptr;

    my_params           .construct_mashed_class();
    my_base_graphs      .construct_mashed_class();
    my_locomotion_graphs.construct_mashed_class();

    resource_context = amalga::resource_manager::get_resource_context();
}

// sub_4CD900
void core_ai_resource::destruct_mashed_class() {
    my_params           .destruct_mashed_class();
    my_base_graphs      .destruct_mashed_class();
    my_locomotion_graphs.destruct_mashed_class();
}

// sub_4DAC20
void core_ai_resource::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    my_params           .unmash(mash_info, this, buffer);
    my_base_graphs      .unmash(mash_info, this, buffer);
    my_locomotion_graphs.unmash(mash_info, this, buffer);

    custom_unmash(mash_info, containing_class_ptr, buffer);
}

// sub_4C7310
void core_ai_resource::custom_unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type) {
    // the info nodes stay a raw image here; each ai_core copies and unmashes its own
    mash_info->align_buffer(mash::SHARED_BUFFER, 4);
    mash_info->read_from_buffer(mash::SHARED_BUFFER, normal_size_of_my_info_nodes);
    mash_info->read_from_buffer(mash::SHARED_BUFFER, shared_size_of_my_info_nodes);

    mash_info->align_buffer(mash::NORMAL_BUFFER, 16);
    mash_info->align_buffer(mash::SHARED_BUFFER, 16);

    my_info_nodes           = (mash::vector<info_node>*)mash_info->get_curr_buffer_pos(mash::NORMAL_BUFFER);
    shared_info_node_buffer = mash_info->get_curr_buffer_pos(mash::SHARED_BUFFER);

    mash_info->advance_buffer(mash::NORMAL_BUFFER, (i32)normal_size_of_my_info_nodes);
    mash_info->advance_buffer(mash::SHARED_BUFFER, (i32)shared_size_of_my_info_nodes);

    mash_info->align_buffer  (mash::NORMAL_BUFFER, 4);
    mash_info->advance_buffer(mash::NORMAL_BUFFER, 4);
    mash_info->align_buffer  (mash::SHARED_BUFFER, 4);
    mash_info->advance_buffer(mash::SHARED_BUFFER, 4);
}
