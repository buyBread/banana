#pragma once

#include <array>
#include <cstddef>

#include "treyarch/ngl/d3d9/shader_program.hh"

namespace treyarch { namespace ngl { namespace shaders {
    inline bool create_program_pair(d3d9::vertex_program &vertex_program, e_shader_program vertex_key,
                                    d3d9::pixel_program  &pixel_program,  e_shader_program pixel_key) {

        return vertex_program.create({ vertex_key }) &&
               pixel_program .create({ pixel_key });
    }

    template<typename T, size_t count>
    bool create_program_range(std::array<T, count> &programs, e_shader_program key) {
        for (size_t index = 0; index < programs.size(); ++index) {
            if (!programs[index].create({ key, (u16)index }))
                return false;
        }

        return true;
    }

    template<typename T, size_t count, typename Variant>
    auto get_program(const std::array<T, count> &programs, Variant variant) -> decltype(programs[0].get()) {
        size_t index = (size_t)variant;

        return index < programs.size() ? programs[index].get() : nullptr;
    }
}}} // treyarch::ngl::shaders
