#include <d3d9.h>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace references {
    util::memory_reference<const D3DVERTEXELEMENT9> quad_position_color_elements     { 0x00F535E4 };
    util::memory_reference<const D3DVERTEXELEMENT9> quad_position_uv_elements        { 0x00F53610 };
    util::memory_reference<const D3DVERTEXELEMENT9> quad_position_color_uv_elements  { 0x00F53628 };
    util::memory_reference<const D3DVERTEXELEMENT9> quad_position_color_uv4_elements { 0x00F53648 };
}}}} // treyarch::ngl::d3d9::references

using namespace treyarch;

void initialize_vertex_format(ngl::vertex_definition  &definition,
                                     u32                      vertex_size,
                                     const D3DVERTEXELEMENT9* elements) {

    definition.vertex_size = vertex_size;
    definition.elements    = elements;
    definition.declaration = nullptr;

    ngl::d3d9::references::device.get()
        ->CreateVertexDeclaration(elements, &definition.declaration);
}

void ngl::d3d9::initialize_internal_vertex_formats() {
    initialize_vertex_format( references::quad_position_color_format.get(),
                              16,
                             &references::quad_position_color_elements.get());

    initialize_vertex_format( references::quad_position_uv_format.get(),
                              20,
                             &references::quad_position_uv_elements.get());

    initialize_vertex_format( references::quad_position_color_uv_format.get(),
                              24,
                             &references::quad_position_color_uv_elements.get());

    initialize_vertex_format( references::quad_position_color_uv4_format.get(),
                              44,
                             &references::quad_position_color_uv4_elements.get());
}
