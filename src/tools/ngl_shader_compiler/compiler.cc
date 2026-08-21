#include <array>
#include <d3dcompiler.h>

#include "treyarch/ngl/shaders/compiler.hh"

using namespace treyarch;
using namespace ngl::shaders;

bool ngl::shaders::compile_shader(const shader_description    &description,
                                  const void*                  source,
                                        size_t                 source_size,
                                  const char*                  source_path,
                                        shader_compile_result &output) {

    std::array<D3D_SHADER_MACRO, maximum_shader_definition_count + 1> definitions {};

    for (size_t index = 0; index < description.definition_count; ++index) {
        definitions[index].Name       = description.definitions[index].name;
        definitions[index].Definition = description.definitions[index].value;
    }

    ID3DBlob* bytecode = nullptr;
    ID3DBlob* errors   = nullptr;

    HRESULT result = D3DCompile(source,
                                source_size,
                                source_path,
                                definitions.data(),
                                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                description.entry_point,
                                description.profile,
                                D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3,
                                0,
                                &bytecode,
                                &errors);

    output.result     = (u32)result;
    output.bytecode   .clear();
    output.diagnostics.clear();

    if (errors) {
        output.diagnostics.assign((const char*)errors->GetBufferPointer(),
                                  errors->GetBufferSize());

        errors->Release();
    }

    if (FAILED(result)) {
        if (bytecode)
            bytecode->Release();

        return false;
    }

    const u8* data = (const u8*)bytecode->GetBufferPointer();
    
    output.bytecode.assign(data, data + bytecode->GetBufferSize());
    
    bytecode->Release();
    
    return true;
}
