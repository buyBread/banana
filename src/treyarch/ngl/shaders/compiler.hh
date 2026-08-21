#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "treyarch/ngl/shaders/description.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders {
    struct shader_compile_result {
        u32             result = 0;
        std::vector<u8> bytecode;
        std::string     diagnostics;
    };

    bool compile_shader(const shader_description    &description,
                        const void*                  source,
                              size_t                 source_size,
                        const char*                  source_path,
                              shader_compile_result &output);
}}} // treyarch::ngl::shaders
