#include "banana/logging.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/d3d9/shader_program_cache.hh"
#include "treyarch/ngl/shaders/package_data.hh"
#include "util/types.hh"

using namespace treyarch;

template<typename T>
bool create_shader_program(      ngl::shaders::shader_key     key,
                                 ngl::shaders::e_shader_stage expected_stage,
                           const char*                        stage_name,
                                 HRESULT                    (*create_program)(const DWORD*, T**),
                                 T**                          output) {

    ngl::shaders::shader_description description {};

    if (!ngl::shaders::describe_shader(key, description) || description.stage != expected_stage)
        return false;

    std::span<const u8> bytecode = ngl::shaders::package_data::load(key);

    if (bytecode.empty()) {
        banana::log.err("failed to load {} program \"{}\"",
                        stage_name,
                        description.source_name);
        
        return false;
    }

    HRESULT result = create_program((const DWORD*)bytecode.data(), output);

    if (FAILED(result)) {
        banana::log.err("failed to create {} program \"{}\" (0x{:08X})",
                        stage_name,
                        description.source_name,
                        (u32)result);

        return false;
    }

    return true;
}

bool ngl::d3d9::vertex_program::create(shaders::shader_key key) {
    return create_shader_program(key,
                                 shaders::e_shader_stage::vertex,
                                 "vertex",
                                 shader_program_cache::create_vertex_program,
                                 &shader_);
}

IDirect3DVertexShader9* ngl::d3d9::vertex_program::get() const {
    return shader_;
}

bool ngl::d3d9::pixel_program::create(shaders::shader_key key) {
    return create_shader_program(key,
                                 shaders::e_shader_stage::pixel,
                                 "pixel",
                                 shader_program_cache::create_pixel_program,
                                 &shader_);
}

IDirect3DPixelShader9* ngl::d3d9::pixel_program::get() const {
    return shader_;
}
