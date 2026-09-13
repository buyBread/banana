#include <cstdlib>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/shader_program_cache.hh"
#include "treyarch/shared/hash/algo.hh"

using namespace treyarch;
using namespace treyarch::ngl;

u32 shader_bytecode_size(const DWORD* bytecode) {
    const DWORD* instruction = bytecode + 1;

    while ((*instruction & D3DSI_OPCODE_MASK) != D3DSIO_END) {
        DWORD token = *instruction;

        if ((token & D3DSI_OPCODE_MASK) == D3DSIO_COMMENT) {
            u32 comment_size = (token & D3DSI_COMMENTSIZE_MASK) >> D3DSI_COMMENTSIZE_SHIFT;

            instruction += comment_size + 1;
        } else {
            u32 operand_count = (token & D3DSI_INSTLENGTH_MASK) >> D3DSI_INSTLENGTH_SHIFT;

            instruction += operand_count + 1;
        }
    }

    return (u32)(instruction - bytecode + 1) * sizeof(DWORD);
}

u32 d3d9::shader_program_cache::shader_crc_traits::operator()(const u32 &crc) const noexcept {
    div_t division = std::div((i32)(crc ^ 0xDEADBEEFu), 127773);

    i32 mixed = 16807 * division.rem - 2836 * division.quot;

    if (mixed < 0)
        mixed += I32_MAX;

    return (u32)mixed;
}

template<typename T, typename create_program_t>
HRESULT create_shader_program(      d3d9::shader_program_cache::program_cache<T> &cache,
                              const DWORD*                                        bytecode,
                                    T**                                           output,
                                    create_program_t                              create_program) {

    u32 byte_count = shader_bytecode_size(bytecode);
    u32 crc        = hash::crc2(bytecode, byte_count, U32_MAX);

    auto* node = cache.find(crc);

    if (node != cache.end()) {
        // the cache owns this, don't AddRef it here
        *output = node->value.value;

        return D3D_OK;
    }

    HRESULT result = create_program(bytecode, output);

    // yes, retail caches it even when creation fails
    cache.insert(crc, *output);

    return result;
}

HRESULT d3d9::shader_program_cache::create_vertex_program(const DWORD*                   bytecode,
                                                                IDirect3DVertexShader9** output) {

    return create_shader_program(references::vertex_programs.get(),
                                 bytecode,
                                 output,
                                 [](const DWORD* data, IDirect3DVertexShader9** shader) {
                                     return d3d9::references::device.read()->CreateVertexShader(data, shader);
                                 });
}

HRESULT d3d9::shader_program_cache::create_pixel_program(const DWORD*                  bytecode,
                                                               IDirect3DPixelShader9** output) {

    return create_shader_program(references::pixel_programs.get(),
                                 bytecode,
                                 output,
                                 [](const DWORD* data, IDirect3DPixelShader9** shader) {
                                     return d3d9::references::device.read()->CreatePixelShader(data, shader);
                                 });
}
