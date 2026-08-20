#include "treyarch/ngl/vertex_definition/vertex_definition.hh"

using namespace treyarch;

void ngl::initialize_vertex_definition_bank() {
    references::vertex_definitions.get().initialize();
}
