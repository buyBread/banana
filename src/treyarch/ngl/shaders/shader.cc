#include "treyarch/ngl/shaders/shader.hh"

using namespace treyarch;

ngl::shader& ngl::get_default_shader() {
    return references::default_shader.get();
}

ngl::shader* ngl::find_shader(string_hash name) {
    return references::shaders.get().find(name);
}

void ngl::initialize_shader_bank() {
    references::shaders.get().initialize();
}
