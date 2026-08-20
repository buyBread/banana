#pragma once

#include <array>
#include <cstddef>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_decal {
    struct material_configuration {
        bool horizon;
        bool gobo;
        bool environment;

        bool operator==(const material_configuration&) const = default;
    };

    enum class e_utility_variant {
        facing,
        sample_texture_0,
        sample_texture_1,
        ambient_alpha,
    };
    
    // todo: clarify with an actual enum class
    inline constexpr size_t material_configuration_count = 8;

    inline constexpr std::array
        <material_configuration, material_configuration_count>
    material_configurations {{
        { false, false, false },
        { false, false, true  },
        { true,  false, false },
        { true,  false, true  },
        { false, true,  false },
        { false, true,  true  },
        { true,  true,  false },
        { true,  true,  true  },
    }};
}}}} // treyarch::ngl::shaders::sm_decal
