#include "treyarch/ngl/d3d9/vertex_definition.hh"

using namespace treyarch;

void ngl::initialize_vertex_definition_bank() {
    references::vertex_definitions.get().initialize();
}
