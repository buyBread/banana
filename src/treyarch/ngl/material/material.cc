#include "banana/logging.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

ngl::material& ngl::get_default_material() {
    return references::default_material.get();
}

// sub_9DB0A0
void ngl::initialize_default_material(shader* empty_shader) {
          material     &value = references::default_material.get();
    const fixed_string &name  = references::default_material_name.get();

    value.name.text   = name.text;
    value.name.hash   = name.hash;
    value.shader_data = empty_shader;
}

// sub_9DC630
void ngl::process_material(material* value) {
    string_hash shader_name((u32)value->shader_data);
    
    shader* material_shader = find_shader(shader_name);

    if (material_shader && material_shader->check_material_version(value))
        value->shader_data = material_shader;
    else {
        const char* material_name = value->name.text ? value->name.text : "(null)";

        if (!material_shader)
            banana::log.ngl("unable to find shader 0x{:08X}, used by material \"{}\" -- assigning default shader",
                            shader_name.source_hash_code,
                            material_name);
        else
            banana::log.ngl("shader \"{}\" rejected material \"{}\" (version {}) -- assigning default shader",
                            material_shader->get_name().text ? material_shader->get_name().text : "(null)",
                            material_name,
                            value->binary_version);

        value->shader_data = &get_default_shader();
    }

    material_shader = value->shader_data;
    material_shader->bind_material(value);
}

void ngl::initialize_material_directory() {
    references::materials.get().initialize();
}

// sub_9DC6B0
void ngl::load_material(amalga::apkf::file*       owner,
                        amalga::apkf::file_entry* entry,
                        void**                    mapped_sections,
                        void*                     user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    
    material* value = (material*)mapped_sections[image_section];

    process_material(value);

    if (!references::resource_callback.read())
        references::materials.get().insert(value);
}

// sub_9DC5F0
void ngl::remove_material(amalga::apkf::file*       owner,
                          amalga::apkf::file_entry* entry,
                          void**                    mapped_sections,
                          void*                     user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    
    material* value = (material*)mapped_sections[image_section];

    if (value->shader_data)
        value->shader_data->release_material(value);

    if (!references::resource_callback.read())
        references::materials.get().erase(value);
}
