#include "banana/logging.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/pcuv/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::pcuv;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

static const D3DVERTEXELEMENT9 vertex_elements[] {
    { 0, 0,  D3DDECLTYPE_SHORT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 8,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
    { 0, 12, D3DDECLTYPE_SHORT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END(),
};

static bool create_vertex_declaration() {
    ngl::vertex_definition &format = program_exports::pcuv::format.get();

    format.vertex_size = 16;
    format.elements    = vertex_elements;
    format.declaration = nullptr;

    HRESULT result = ngl::d3d9::references::device.read()->CreateVertexDeclaration(format.elements, &format.declaration);

    if (FAILED(result)) {
        banana::log.err("failed to create the compressed PCUV vertex declaration (0x{:08X})", (u32)result);

        return false;
    }

    return true;
}

bool ngl::shaders::pcuv::initialize() {
    if (!vertex_program.create({ e_shader_program::pcuv_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::pcuv_pixel }))
        return false;

    program_exports::pcuv::vertex_program.write(vertex_program.get());
    program_exports::pcuv::pixel_program.write(pixel_program.get());

    return create_vertex_declaration();
}
