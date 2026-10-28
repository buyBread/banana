#include "retail.hh"
#include "game/wds/entity/interface/script_data_interface.hh"

using namespace treyarch;

bool script_data_interface::has_entity_in_custom_list() {
    return retail::sub_610120((u32*)this);
}

void script_data_interface::add_entity_to_custom_entity_list(arch_base_vhandle ent) {
    retail::sub_643950((u32*)this, (i32)ent.value);
}

void script_data_interface::remove_entity_from_custom_entity_list(arch_base_vhandle ent) {
    retail::sub_63F3E0((u32*)this, (i32)ent.value);
}

arch_base_vhandle script_data_interface::get_first_entity_from_custom_entity_list() {
    arch_base_vhandle first;
    retail::sub_638C90((u32*)this, &first.value);

    return first;
}

i32 script_data_interface::get_custom_entity_list_count() {
    return retail::sub_610140((u32*)this);
}

// sub_8237F0
void script_data_interface::set_als_anim_filename_use_prefix(bool use_prefix) {
    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, "als_anim_filename_use_prefix");

    data.set_pb_float(parameter, use_prefix ? 1.0f : 0.0f);
}
