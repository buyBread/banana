#include "treyarch/shared/four_cc.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

ngl::material& ngl::get_default_material() {
    return references::default_material.get();
}

void ngl::process_material(material* value) {
    string_hash shader_name((u32)value->shader_data);
    
    shader* material_shader = find_shader(shader_name);

    if (material_shader && material_shader->check_material_version(value))
        value->shader_data = material_shader;
    else
        value->shader_data = &get_default_shader();

    material_shader = value->shader_data;
    material_shader->bind_material(value);
}

void ngl::initialize_material_directory() {
    references::materials.get().initialize();
}

void __cdecl ngl::load_material(amalga::file*       owner,
                                amalga::file_entry* entry,
                                void**              mapped_sections,
                                void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    
    material* value = (material*)mapped_sections[image_section];

    process_material(value);

    if (!references::resource_callback.read())
        references::materials.get().insert(value);
}

void __cdecl ngl::remove_material(amalga::file*       owner,
                                  amalga::file_entry* entry,
                                  void**              mapped_sections,
                                  void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    
    material* value = (material*)mapped_sections[image_section];

    if (value->shader_data)
        value->shader_data->release_material(value);

    if (!references::resource_callback.read())
        references::materials.get().erase(value);
}
