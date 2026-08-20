#pragma once

#include <array>
#include <cstddef>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
    enum class e_vertex_variant {
        material,
        utility,
    };

    enum class e_environment_mapping : uint8_t {
        none,
        uniform,
        mapped,
    };

    enum class e_light_count : uint8_t {
        none = 0,
        two  = 2,
        four = 4,
    };

    enum class e_shadow_count : uint8_t {
        none,
        one,
        two,
    };

    enum class e_debug_variant {
        texture_coordinates_0,
        facing,
        texture_coordinates_1,
        gobo,
        normal,
        environment,
        shadow_0,
        shadow_1,
    };

    // todo: clarify with an actual enum class
    inline constexpr size_t material_configuration_count = 17;
    inline constexpr size_t lighting_configuration_count = 21;
    inline constexpr size_t debug_variant_count          = 8;

    struct material_configuration {
        bool                  diffuse;
        bool                  opacity;
        bool                  specularity;
        bool                  specular_exponent;
        bool                  emissiveness;
        bool                  normal;
        e_environment_mapping environment_mapping;

        bool operator==(const material_configuration&) const = default;
    };

    struct lighting_configuration {
        bool           horizon;
        e_light_count  light_count;
        bool           gobo;
        e_shadow_count shadow_count;

        bool operator==(const lighting_configuration&) const = default;
    };

    inline constexpr std::array<material_configuration, material_configuration_count>
    material_configurations {{
        { true,  false, true,  false, false, true,  e_environment_mapping::none    },
        { true,  false, true,  false, false, true,  e_environment_mapping::uniform },
        { true,  false, true,  false, false, false, e_environment_mapping::none    },
        { true,  false, true,  false, false, false, e_environment_mapping::uniform },
        { true,  false, false, false, false, false, e_environment_mapping::none    },
        { true,  false, true,  false, false, true,  e_environment_mapping::mapped  },
        { true,  false, true,  true,  false, true,  e_environment_mapping::mapped  },
        { true,  false, true,  true,  false, true,  e_environment_mapping::none    },
        { false, false, false, false, false, false, e_environment_mapping::none    },
        { true,  false, false, false, false, true,  e_environment_mapping::none    },
        { true,  false, true,  false, true,  false, e_environment_mapping::none    },
        { true,  false, true,  false, false, false, e_environment_mapping::mapped  },
        { true,  false, true,  true,  false, true,  e_environment_mapping::uniform },
        { true,  false, true,  true,  false, false, e_environment_mapping::mapped  },
        { true,  false, false, false, false, true,  e_environment_mapping::uniform },
        { false, false, true,  false, false, false, e_environment_mapping::uniform },
        { true,  true,  true,  true,  true,  true,  e_environment_mapping::mapped  },
    }};

    inline constexpr std::array<lighting_configuration, lighting_configuration_count>
    lighting_configurations {{
        { false, e_light_count::two,  false, e_shadow_count::none },
        { false, e_light_count::two,  false, e_shadow_count::one  },
        { false, e_light_count::two,  false, e_shadow_count::two  },
        { false, e_light_count::two,  true,  e_shadow_count::none },
        { false, e_light_count::two,  true,  e_shadow_count::one  },
        { false, e_light_count::two,  true,  e_shadow_count::two  },
        { false, e_light_count::four, false, e_shadow_count::none },
        { false, e_light_count::four, false, e_shadow_count::one  },
        { false, e_light_count::four, false, e_shadow_count::two  },
        { false, e_light_count::four, true,  e_shadow_count::none },
        { false, e_light_count::four, true,  e_shadow_count::one  },
        { false, e_light_count::four, true,  e_shadow_count::two  },
        { true,  e_light_count::none, false, e_shadow_count::none },
        { true,  e_light_count::none, false, e_shadow_count::one  },
        { true,  e_light_count::none, false, e_shadow_count::two  },
        { true,  e_light_count::four, false, e_shadow_count::none },
        { true,  e_light_count::four, false, e_shadow_count::one  },
        { true,  e_light_count::four, false, e_shadow_count::two  },
        { true,  e_light_count::four, true,  e_shadow_count::none },
        { true,  e_light_count::four, true,  e_shadow_count::one  },
        { true,  e_light_count::four, true,  e_shadow_count::two  },
    }};

    inline constexpr std::array<const char*, debug_variant_count>
    integer_definitions {{
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
    }};

    constexpr uint16_t material_permutation(size_t material, size_t lighting) {
        return (uint16_t)(material * lighting_configuration_count + lighting);
    }
}}}} // treyarch::ngl::shaders::sm_phat
