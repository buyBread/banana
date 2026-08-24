#include "treyarch/ngl/geometry_shader/geometry_shader.hh"
#include "treyarch/shared/hash/algo.hh"

using namespace treyarch;

void ngl::geometry_shader::register_item() {
    id = references::next_geometry_shader_id.read();
    references::next_geometry_shader_id.write(id + 1);

    references::geometry_shaders.get().insert(this);
}

const fixed_string &ngl::morph_geometry_shader::get_name() const {
    u32 guard = references::morph_geometry_shader_name_guard.read();

    if (!(guard & 1)) {
        references::morph_geometry_shader_name_guard.write(guard | 1);

        fixed_string& name = references::morph_geometry_shader_name.get();

        name.text = nullptr;
        name.hash = string_hash(hash::djb2("test"));
    }

    return references::morph_geometry_shader_name.get();
}

string_hash ngl::geometry_shader_name::get(const geometry_shader* value) {
    const morph_geometry_shader* morph = &references::morph_geometry_shader_instance.get();

    if (value == morph)
        return morph->morph_geometry_shader::get_name().hash;

    return value->get_name().hash;
}

void ngl::initialize_geometry_shader_bank() {
    references::geometry_shaders.get().initialize();
}
