#include "treyarch/amalga/file.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/init.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "treyarch/ngl/vertex_definition/vertex_definition.hh"

using namespace treyarch;

void ngl::mesh_init() {
    amalga::register_file_type(four_cc('M', 'E', 'S', 'H'),
                               2,
                               load_mesh,
                               remove_mesh);

    amalga::register_file_type(four_cc('M', 'A', 'T'),
                               2,
                               load_material,
                               remove_material);

    initialize_mesh_directory();
    initialize_material_directory();
    initialize_shader_bank();
    initialize_vertex_definition_bank();

    amalga::register_file_type(four_cc('M', 'O', 'R', 'H'),
                               1,
                               load_morph,
                               remove_morph);

    initialize_morph_directory();
}
