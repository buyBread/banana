#include "treyarch/ngl/morph/morph.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

void ngl::initialize_morph_directory() {
    references::morphs.get().initialize();
}

void __cdecl ngl::load_morph(amalga::file*       owner,
                             amalga::file_entry* entry,
                             void**              mapped_sections,
                             void*               user_data) {

    (void)entry;
    (void)user_data;

    constexpr u32 component_types[] { 0, 3, 7, 6, 10, 5 };

    i32 image_section = owner->find_section_index(string_hash(amalga::four_cc('I', 'M', 'G')));
    
    morph_set* value = (morph_set*)mapped_sections[image_section];

    for (u32 frame_index = 0; frame_index < value->frame_count; ++frame_index) {
        morph_frame &frame = value->frames[frame_index];

        for (u32 section_index = 0; section_index < frame.section_count; ++section_index) {
            morph_section &section = frame.sections[section_index];

            for (u32 component_index = 0;
                 component_index < section.component_count;
                 ++component_index) {

                morph_component& component = section.components[component_index];
                component.type = component_types[component.type];
            }
        }
    }

    if (!references::resource_callback.read())
        references::morphs.get().insert(value);
}

void __cdecl ngl::remove_morph(amalga::file*       owner,
                               amalga::file_entry* entry,
                               void**              mapped_sections,
                               void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(amalga::four_cc('I', 'M', 'G')));
    
    morph_set* value = (morph_set*)mapped_sections[image_section];

    if (!references::resource_callback.read())
        references::morphs.get().erase(value);
}
