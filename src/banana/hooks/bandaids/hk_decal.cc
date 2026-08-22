#include <mutex>
#include <string>
#include <string_view>
#include <vector>
#include <d3d9.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "banana/hooks/base.hh"
#include "treyarch/shared/hash/algo.hh"
#include "util/vtables/IDirect3DDevice9.hh"
#include "util/gimmie/virt.hh"
#include "util/gimmie/fn.hh"
#include "util/macros/lambda.hh"
#include "util/types.hh"

/*
    working:
    - web splats
    - ground cracks (small & big)
    - symbiote splats
    - ???

    missing:
    - wolverine claw marks
        - not decal_basic, apparently
    - web grind ground cracks trail
    - ???
*/

struct decal_shader_pair_t {
    IDirect3DPixelShader9* native;
    IDirect3DPixelShader9* repaired;
};

std::mutex decal_shader_mutex;

std::vector<decal_shader_pair_t> decal_shaders;

bool replace_shader_instruction(std::string      &source,
                                std::string_view instruction,
                                std::string_view replacement) {

    const size_t offset = source.find(instruction);

    if (offset == std::string::npos ||
        source.find(instruction, offset + instruction.size()) != std::string::npos)

        return false;

    source.replace(offset, instruction.size(), replacement);

    return true;
}

bool create_repaired_decal_shader(      IDirect3DDevice9*       device,
                                  const DWORD*                  bytecode,
                                        size_t                  byte_count,
                                        IDirect3DPixelShader9** output) {

    if (!device || !bytecode || !byte_count || !output)
        return false;

    ID3DBlob* disassembly = nullptr;

    const HRESULT disassemble_result = D3DDisassemble(
        bytecode,
        byte_count,
        0,
        nullptr,
        &disassembly);

    if (FAILED(disassemble_result) || !disassembly) {
        if (disassembly)
            disassembly->Release();

        return false;
    }

    std::string source((const char*)disassembly->GetBufferPointer(), disassembly->GetBufferSize());

    disassembly->Release();

    while (!source.empty() && source.back() == '\0')
        source.pop_back();

    if (!replace_shader_instruction(source, "add r6.w, r6.w, c64.x",
            "add r6.w, r6.w, -c64.x"))

        return false;

    if (!replace_shader_instruction(source, "add r0.w, r0.w, c64.y",
            "add r0.w, r0.w, -c64.y"))

        return false;

    if (!replace_shader_instruction(source, "lrp r6.w, r5.y, r3.y, r3.x",
            "lrp r6.w, r5.y, r3.y, r3.x\n"
            "    add r6.w, -r6.w, c66.z"))

        return false;

    if (!replace_shader_instruction(source, "lrp r0.w, r5.y, r3.y, r3.x",
            "lrp r0.w, r5.y, r3.y, r3.x\n"
            "    add r0.w, -r0.w, c66.z"))

        return false;

    using D3DXAssembleShader_t = HRESULT(WINAPI*)(const char*,
                                                        UINT,
                                                  const void*,
                                                        void*,
                                                        DWORD,
                                                        ID3DBlob**,
                                                        ID3DBlob**);

    static auto D3DXAssembleShader = util::gimmie::fn
        <D3DXAssembleShader_t>(store::handle_d3dx, "D3DXAssembleShader");

    ID3DBlob* repaired_bytecode = nullptr;
    ID3DBlob* errors            = nullptr;

    const HRESULT assemble_result = D3DXAssembleShader(source.data(),
                                                       (UINT)source.size(),
                                                       nullptr,
                                                       nullptr,
                                                       0,
                                                       &repaired_bytecode,
                                                       &errors);

    if (FAILED(assemble_result) || !repaired_bytecode) {
        if (errors) {
            banana::log.err("couldn't assemble repaired decal_basic shader (0x{:08X}): {}",
                (u32)assemble_result,
                (const char*)errors->GetBufferPointer());

            errors->Release();
        }

        if (repaired_bytecode)
            repaired_bytecode->Release();

        return false;
    }

    const HRESULT create_result = device->CreatePixelShader(
        (const DWORD*)repaired_bytecode->GetBufferPointer(),
        output);

    if (errors)
        errors->Release();

    repaired_bytecode->Release();
    
    return SUCCEEDED(create_result) && *output;
}

void track_decal_shader(IDirect3DPixelShader9* native,
                        IDirect3DPixelShader9* repaired) {

    std::lock_guard lock(decal_shader_mutex);

    for (const auto& candidate : decal_shaders) {
        if (candidate.native == native) {
            repaired->Release();
            
            return;
        }
    }

    decal_shaders.push_back({ native, repaired });
}

bool is_decal_shader(IDirect3DPixelShader9* shader) {
    if (!shader)
        return false;

    std::lock_guard lock(decal_shader_mutex);

    for (const auto& candidate : decal_shaders)
        if (candidate.native == shader)
            return true;

    return false;
}

IDirect3DPixelShader9* get_repaired_decal_shader(IDirect3DPixelShader9* native) {
    if (!native)
        return nullptr;

    std::lock_guard lock(decal_shader_mutex);

    for (const auto& candidate : decal_shaders)
        if (candidate.native == native)
            return candidate.repaired;

    return nullptr;
}

IDirect3DPixelShader9* begin_decal_draw(IDirect3DDevice9* device) {
    if (!device)
        return nullptr;

    IDirect3DPixelShader9* native = nullptr;

    if (FAILED(device->GetPixelShader(&native)) || !native)
        return nullptr;

    auto* repaired = get_repaired_decal_shader(native);

    if (!repaired) {
        native->Release();
        
        return nullptr;
    }

    if (FAILED(device->SetPixelShader(repaired))) {
        native->Release();
        
        return nullptr;
    }

    return native;
}

void end_decal_draw(IDirect3DDevice9* device, IDirect3DPixelShader9* native) {
    if (!native)
        return;

    if (device)
        device->SetPixelShader(native);

    native->Release();
}

IDirect3DBaseTexture9* get_depth_texture() {
    auto* ngl_current_scene = *(u8**)0x01115C54;
    auto* ngl_backbuffer    = *(u8**)0x01123A1C;

    constexpr size_t ngl_depth_buffer_offset           = 56;
    constexpr size_t ngl_texture_offset                = 20;
    constexpr size_t current_scene_depth_buffer_offset = 976;

    auto* current_scene_wrapper = ngl_current_scene ?
        *(u8**)(ngl_current_scene + current_scene_depth_buffer_offset) : nullptr;

    auto* backbuffer_wrapper = ngl_backbuffer ?
        *(u8**)(ngl_backbuffer + ngl_depth_buffer_offset) : nullptr;

    auto* selected_wrapper = current_scene_wrapper ?
        current_scene_wrapper : backbuffer_wrapper;

    auto* texture = selected_wrapper ?
        *(IDirect3DBaseTexture9**)(selected_wrapper + ngl_texture_offset) : nullptr;

    return texture;
}

/*
    setup_pixel_shader
*/

DEFINE_HOOK(hk_sub_9DDF10, hook_signatures::bandaids::sub_9DDF10) {

public:
    CONSTRUCT_HOOK(hk_sub_9DDF10, (void*)0x9DDF10, "bandaids") {}

    static int __cdecl detour(const DWORD* bytecode, IDirect3DPixelShader9** shader) {
        auto result = m_original(bytecode, shader);

        if (!bytecode || !shader || !*shader)
            return result;

        using D3DXGetShaderSize_t = UINT(WINAPI*)(const DWORD* bytecode);

        static auto D3DXGetShaderSize = util::gimmie::fn
            <D3DXGetShaderSize_t>(store::handle_d3dx, "D3DXGetShaderSize");

        const auto byte_count = D3DXGetShaderSize(bytecode);

        if (treyarch::hash::crc2((u8*)bytecode, byte_count) != 0x318FF857) // decal basic pixel shader crc
            return result;

        IDirect3DPixelShader9* repaired_shader = nullptr;

        if (!create_repaired_decal_shader(store::d3d9_device,
                                          bytecode,
                                          byte_count,
                                          &repaired_shader)) {

            HK_ERR("failed to repair decal_basic pixel shader {:p}", (void*)*shader);

            return result;
        }

        track_decal_shader(*shader, repaired_shader);

        return result;
    }
};

/*
    apply_node
*/

DEFINE_HOOK(hk_sub_9EA950, hook_signatures::bandaids::sub_9EA950) {

    static void apply_on_decal(DWORD* node) {
        if (!node)
            return;

        constexpr size_t node_shader_state_offset = 220;

        auto* shader_state = *(u8**)((u8*)node + node_shader_state_offset);

        if (!shader_state)
            return;

        constexpr size_t shader_state_pixel_shader_offset = 36;

        auto* node_pixel_shader = *(IDirect3DPixelShader9**)(shader_state + shader_state_pixel_shader_offset);

        if (!is_decal_shader(node_pixel_shader))
            return;

        constexpr DWORD zbuffer_sampler = 8;

        auto* depth_texture = get_depth_texture();

        if (!depth_texture)
            return;

        store::d3d9_device->SetTexture(zbuffer_sampler, depth_texture);
    }

public:
    CONSTRUCT_HOOK(hk_sub_9EA950, (void*)0x9EA950, "bandaids") {}

    static int __cdecl detour(int renderer, DWORD* node) {
        auto result = m_original(renderer, node);

        apply_on_decal(node);

        return result;
    }
};

/*
    IDirect3DDevice9::DrawIndexedPrimitive
*/

DEFINE_HOOK(hk_DrawIndexedPrimitive, hook_signatures::bandaids::DrawIndexedPrimitive) {

public:
    CONSTRUCT_HOOK(hk_DrawIndexedPrimitive,
        LAMBDA {
            return util::gimmie::virt(
                store::d3d9_device,
                util::vtables::IDirect3DDevice9::DrawIndexedPrimitive);
        }, "bandaids") {}

    static HRESULT WINAPI detour(IDirect3DDevice9* device,
                                 D3DPRIMITIVETYPE  primitive_type,
                                 INT               base_vertex_index,
                                 UINT              min_vertex_index,
                                 UINT              vertex_count,
                                 UINT              start_index,
                                 UINT              primitive_count) {

        auto* native = begin_decal_draw(device);
        
        const HRESULT result = m_original(device,
                                          primitive_type,
                                          base_vertex_index,
                                          min_vertex_index,
                                          vertex_count,
                                          start_index,
                                          primitive_count);
        
        end_decal_draw(device, native);
        
        return result;
    }
};

CREATE_HOOK(hk_sub_9DDF10);
CREATE_HOOK(hk_sub_9EA950);
CREATE_HOOK(hk_DrawIndexedPrimitive);