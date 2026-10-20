#include "retail.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9D1800
void nal::init() {
    memory::initialize_stack_bounds();

    auto null_callback = (amalga::merged_apk::file_callback)retail::sub_5B4A70;

    amalga::merged_apk::register_file_type(transport_list_type,
                                           transport_list_version,
                                           null_callback,
                                           null_callback);

    amalga::merged_apk::register_file_type(anim_type,
                                           anim_version,
                                           load_anim,
                                           remove_anim);

    amalga::merged_apk::register_file_type(scene_anim_type,
                                           scene_anim_version,
                                           load_scene_anim,
                                           null_callback);
}

// sub_9CD550
nal::transport_list* nal::get_skeleton(fixed_string* name) {
    return (transport_list*)amalga::merged_apk::resolve_resource(name, transport_list_type);
}

// sub_9CD570
nal::anim* nal::get_anim(fixed_string* name) {
    return (anim*)amalga::merged_apk::resolve_resource(name, anim_type);
}

// sub_9CD590
nal::scene_anim* nal::get_scene_anim(fixed_string* name) {
    return (scene_anim*)amalga::merged_apk::resolve_resource(name, scene_anim_type);
}

// sub_9D1790
void nal::load_anim(amalga::merged_apk::file*       owner,
                    amalga::merged_apk::file_entry* entry,
                    void**                          mapped_sections,
                    void*                           user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    if (references::skip_linking.read())
        return;

    for (anim* value = (anim*)mapped_sections[0]; value; value = value->next) {
        value->flags     |= anim_flag_unk_04;
        value->ref_count  = 1;

        value->link_transport_list();
    }
}

// sub_9CFAF0
void nal::remove_anim(amalga::merged_apk::file*       owner,
                      amalga::merged_apk::file_entry* entry,
                      void**                          mapped_sections,
                      void*                           user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    // each anim's release check is gone, only the walk is left
    for (anim* value = (anim*)mapped_sections[0]; value; value = value->next) {}
}

// sub_9D1770
void nal::load_scene_anim(amalga::merged_apk::file*       owner,
                          amalga::merged_apk::file_entry* entry,
                          void**                          mapped_sections,
                          void*                           user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    if (references::skip_linking.read())
        return;

    ((scene_anim*)mapped_sections[0])->link_transport_lists();
}
