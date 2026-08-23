#include "banana/logging.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/package_data.hh"
#include "util/types.hh"

using namespace treyarch;

bool ngl::d3d9::vertex_program::create(shaders::shader_key key) {
    shaders::shader_description description {};

    if (!shaders::describe_shader(key, description) || description.stage != shaders::e_shader_stage::vertex)
        return false;

    std::span<const u8> bytecode = shaders::package_data::load(key);

    if (bytecode.empty()) {
        banana::log.err("failed to load vertex program \"{}\"", description.source_name);
        
        return false;
    }

    if (shader_) {
        shader_->Release();
        shader_ = nullptr;
    }

    HRESULT result = references::device.read()
        ->CreateVertexShader((const DWORD*)bytecode.data(), &shader_);

    if (FAILED(result)) {
        banana::log.err("failed to create vertex program \"{}\" (0x{:08X})",
            description.source_name,
            (u32)result);

        return false;
    }

    return true;
}

IDirect3DVertexShader9* ngl::d3d9::vertex_program::get() const {
    return shader_;
}

bool ngl::d3d9::pixel_program::create(shaders::shader_key key) {
    shaders::shader_description description {};

    if (!shaders::describe_shader(key, description) || description.stage != shaders::e_shader_stage::pixel)
        return false;

    std::span<const u8> bytecode = shaders::package_data::load(key);

    if (bytecode.empty()) {
        banana::log.err("failed to load pixel program \"{}\"", description.source_name);
        
        return false;
    }

    if (shader_) {
        shader_->Release();
        shader_ = nullptr;
    }

    HRESULT result = references::device.read()
        ->CreatePixelShader((const DWORD*)bytecode.data(), &shader_);

    if (FAILED(result)) {
        banana::log.err("failed to create pixel program \"{}\" (0x{:08X})",
            description.source_name,
            (u32)result);
        
        return false;
    }

    return true;
}

IDirect3DPixelShader9* ngl::d3d9::pixel_program::get() const {
    return shader_;
}
