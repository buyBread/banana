#pragma once

#include <array>
#include <cstddef>

#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders {
    enum class e_shader_stage : u8 {
        vertex,
        pixel,
    };

    enum class e_shader_program : u16 {
        pcuv_vertex,
        pcuv_pixel,
        puv_vertex,
        puv_pixel,
        sm_simple_vertex,
        sm_simple_pixel,
        sm_translucent_vertex,
        sm_translucent_pixel,
        sm_depth_shadow_vertex,
        sm_depth_shadow_pixel,
        smsky_vertex,
        smsky_daylight_pixel,
        smsky_night_pixel,
        smsky_combined_pixel,
        road_lights_vertex,
        road_lights_pixel,
        fake_peds_vertex,
        fake_peds_pixel,
        sm_citylod_vertex,
        sm_citylod_pixel,
        sm_retrofit_vertex,
        sm_retrofit_pixel,
        sm_buildinglod_vertex,
        sm_buildinglod_adaptive_alpha_pixel,
        sm_buildinglod_opaque_pixel,
        sm_roadlod_vertex,
        sm_roadlod_surface_pixel,
        sm_roadlod_sampler_passthrough_pixel,
        sm_decalchar_material_vertex,
        sm_decalchar_depth_shadow_vertex,
        sm_decalchar_pixel,
        sm_bush_material_vertex,
        sm_bush_material_pixel,
        sm_bush_shadowed_material_pixel,
        sm_bush_constant_red_pixel,
        sm_bush_sample_texture_pixel,
        sm_bush_sample_auxiliary_texture_pixel,
        sm_babyphatnormal_material_vertex,
        sm_babyphatnormal_depth_shadow_vertex,
        sm_babyphatnormal_material_pixel,
        sm_babyphat_material_vertex,
        sm_babyphat_depth_shadow_vertex,
        sm_babyphat_material_pixel,
        sm_decal_vertex,
        sm_decal_material_pixel,
        sm_decal_facing_pixel,
        sm_decal_ambient_alpha_pixel,
        sm_phat_palette_normal_material_vertex,
        sm_phat_palette_normal_depth_shadow_vertex,
        sm_phat_palette_normal_material_pixel,
        sm_phat_palette_normal_sampler_passthrough_pixel,
        sm_phatnormal_material_vertex,
        sm_phatnormal_depth_shadow_vertex,
        sm_phat_material_vertex,
        sm_phat_material_pixel,
        sm_phat_debug_pixel,
        sm_road_vertex,
        sm_road_material_pixel,
        sm_road_sample_auxiliary_alpha_pixel,
        sm_phatpalettecharnormal_material_vertex,
        sm_phatpalettecharnormal_depth_shadow_vertex,
        sm_phatpalettecharnormal_material_pixel,
        sm_phatpalettecharnormal_sample_auxiliary_alpha_pixel,
        sm_phatpalettechar_material_pixel,
        sm_bright_filter_vertex,
        sm_bright_filter_pixel,
        sm_phat_palette_material_vertex,
        sm_phat_palette_material_pixel,
        sm_phat_palette_sample_texture_pixel,
        sm_phatcharnormal_material_vertex,
        sm_phatcharnormal_material_pixel,
        sm_phatcharnormal_sample_texture_pixel,
        sm_phatcharnormal_sample_green_pixel,
        sm_phatchar_material_pixel,
        sm_phatspiderman_material_vertex,
        sm_phatspiderman_depth_shadow_vertex,
        sm_phatspiderman_material_pixel,
        sm_phatspiderman_sample_texture_pixel,
        sm_phatspiderman_sample_green_pixel,
        scene_color_copy_vertex,
        scene_color_copy_pixel,
        scene_color_sanitize_pixel,
        count,
    };

    struct shader_key {
        e_shader_program program;
        u16              permutation = 0;
    };

    struct shader_definition {
        const char* name;
        const char* value;
    };

    inline constexpr size_t maximum_shader_definition_count = 11;

    struct shader_description {
        shader_key                    key;
        e_shader_stage                stage;
        const char*                   source_path;
        const char*                   source_name;
        const char*                   entry_point;
        const char*                   profile;
        std::array<shader_definition, maximum_shader_definition_count> definitions {};
        size_t                        definition_count = 0;
    };

    u32 encode_shader_key(shader_key key);
    
    size_t get_permutation_count(e_shader_program program);
    
    bool describe_shader(shader_key key, shader_description &description);
}}} // treyarch::ngl::shaders
