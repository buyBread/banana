#include "treyarch/ngl/shaders/sm_decal/configuration.hh"
#include "treyarch/ngl/shaders/sm_bright_filter/configuration.hh"
#include "treyarch/ngl/shaders/sm_phat/configuration.hh"

#include "treyarch/ngl/shaders/description.hh"

using namespace treyarch;
using namespace ngl::shaders;

static constexpr std::array<const char*, 3>
sm_babyphatnormal_shadow_definitions {{
    "0",
    "1",
    "2",
}};

static constexpr std::array<const char*, 3>
sm_babyphat_shadow_definitions {{
    "0",
    "1",
    "2",
}};

static constexpr std::array<const char*, 3>
sm_phatspiderman_shadow_definitions {{
    "0",
    "1",
    "2",
}};

static constexpr std::array<const char*, 3>
sm_phat_palette_normal_shadow_definitions {{
    "0",
    "1",
    "2",
}};

static constexpr std::array<const char*, 3>
sm_road_shadow_definitions {{
    "0",
    "1",
    "2",
}};

static void describe(      shader_description &description,
                           shader_key          key,
                           e_shader_stage      stage,
                     const char*               source_path,
                     const char*               source_name,
                     const char*               profile) {

    description.key          = key;
    description.stage        = stage;
    description.source_path  = source_path;
    description.source_name  = source_name;
    description.entry_point  = "main";
    description.profile      = profile;
    description.definitions  = {};
    description.definition_count = 0;
}

static const char* definition(bool value) {
    return sm_phat::integer_definitions[value ? 1 : 0];
}

template <typename T>
static const char* definition(T value) {
    return sm_phat::integer_definitions[(size_t)value];
}

static bool describe_sm_phat(shader_key key, shader_description &description) {
    switch (key.program) {
        case e_shader_program::sm_phat_material_vertex:
            if (key.permutation)
                return false;

            describe(description, key, e_shader_stage::vertex,
                     "sm_phat/sm_phat.material.vs.hlsl",
                     "sm_phat.material.vs.hlsl",
                     "vs_3_0");

            return true;

        case e_shader_program::sm_phat_material_pixel: {
            if (key.permutation >= sm_phat::material_configuration_count * sm_phat::lighting_configuration_count)
                return false;

            size_t material_index = key.permutation / sm_phat::lighting_configuration_count;
            size_t lighting_index = key.permutation % sm_phat::lighting_configuration_count;
            const sm_phat::material_configuration &material =
                sm_phat::material_configurations[material_index];
            const sm_phat::lighting_configuration &lighting =
                sm_phat::lighting_configurations[lighting_index];

            describe(description, key, e_shader_stage::pixel,
                     "sm_phat/sm_phat.material.ps.hlsl",
                     "sm_phat.material.ps.hlsl",
                     "ps_3_0");

            description.definitions = {{
                { "NGL_SM_PHAT_DIFFUSE",           definition(material.diffuse) },
                { "NGL_SM_PHAT_OPACITY",           definition(material.opacity) },
                { "NGL_SM_PHAT_SPECULARITY",       definition(material.specularity) },
                { "NGL_SM_PHAT_SPECULAR_EXPONENT", definition(material.specular_exponent) },
                { "NGL_SM_PHAT_EMISSIVENESS",      definition(material.emissiveness) },
                { "NGL_SM_PHAT_NORMAL",            definition(material.normal) },
                { "NGL_SM_PHAT_ENVIRONMENT",       definition(material.environment_mapping) },
                { "NGL_SM_PHAT_HORIZON",           definition(lighting.horizon) },
                { "NGL_SM_PHAT_LIGHT_COUNT",       definition(lighting.light_count) },
                { "NGL_SM_PHAT_GOBO",              definition(lighting.gobo) },
                { "NGL_SM_PHAT_SHADOW_COUNT",      definition(lighting.shadow_count) },
            }};
            description.definition_count = description.definitions.size();

            return true;
        }

        case e_shader_program::sm_phat_debug_pixel:
            if (key.permutation >= sm_phat::debug_variant_count)
                return false;

            describe(description, key, e_shader_stage::pixel,
                "sm_phat/sm_phat.debug.ps.hlsl",
                "sm_phat.debug.ps.hlsl",
                "ps_3_0");
            description.definitions[0] = {
                "NGL_SM_PHAT_DEBUG_VARIANT",
                sm_phat::integer_definitions[key.permutation]
            };
            description.definition_count = 1;

            return true;

        default:
            return false;
    }
}

u32 ngl::shaders::encode_shader_key(shader_key key) {
    return ((u32)key.program << 16) | key.permutation;
}

size_t ngl::shaders::get_permutation_count(e_shader_program program) {
    switch (program) {
        case e_shader_program::sm_phat_material_pixel:
            return sm_phat::material_configuration_count * sm_phat::lighting_configuration_count;
        case e_shader_program::sm_phat_debug_pixel:
            return sm_phat::debug_variant_count;
        case e_shader_program::sm_babyphatnormal_material_pixel:
            return sm_babyphatnormal_shadow_definitions.size();
        case e_shader_program::sm_babyphat_material_pixel:
            return sm_babyphat_shadow_definitions.size();
        case e_shader_program::sm_decal_material_pixel:
            return sm_decal::material_configuration_count;
        case e_shader_program::sm_phat_palette_normal_material_pixel:
            return sm_phat_palette_normal_shadow_definitions.size();
        case e_shader_program::sm_road_material_pixel:
            return sm_road_shadow_definitions.size();
        case e_shader_program::sm_phatpalettecharnormal_material_pixel:
            return sm_phat_palette_normal_shadow_definitions.size();
        case e_shader_program::sm_phatpalettechar_material_pixel:
            return sm_phat_palette_normal_shadow_definitions.size();
        case e_shader_program::sm_bright_filter_vertex:
            return sm_bright_filter::vertex_configurations.size();
        case e_shader_program::sm_bright_filter_pixel:
            return sm_bright_filter::pixel_configurations.size();
        case e_shader_program::sm_phat_palette_material_pixel:
            return sm_phat_palette_normal_shadow_definitions.size();
        case e_shader_program::sm_phatcharnormal_material_pixel:
            return sm_babyphatnormal_shadow_definitions.size();
        case e_shader_program::sm_phatchar_material_pixel:
            return sm_babyphat_shadow_definitions.size();
        case e_shader_program::sm_phatspiderman_material_pixel:
            return sm_phatspiderman_shadow_definitions.size();
        case e_shader_program::count:
            return 0;
        default:
            return 1;
    }
}

bool ngl::shaders::describe_shader(shader_key key, shader_description &description) {
    if (key.permutation >= get_permutation_count(key.program))
        return false;

    switch (key.program) {
        case e_shader_program::pcuv_vertex:
            describe(description, key, e_shader_stage::vertex,
                "pcuv/pcuv.vs.hlsl", "pcuv.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::pcuv_pixel:
            describe(description, key, e_shader_stage::pixel,
                "pcuv/pcuv.ps.hlsl", "pcuv.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::puv_vertex:
            describe(description, key, e_shader_stage::vertex,
                "puv/puv.vs.hlsl", "puv.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::puv_pixel:
            describe(description, key, e_shader_stage::pixel,
                "puv/puv.ps.hlsl", "puv.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_simple_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_simple/sm_simple.vs.hlsl", "sm_simple.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_simple_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_simple/sm_simple.ps.hlsl", "sm_simple.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_translucent_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_translucent/sm_translucent.vs.hlsl", "sm_translucent.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_translucent_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_translucent/sm_translucent.ps.hlsl", "sm_translucent.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_depth_shadow/sm_depth_shadow.vs.hlsl", "sm_depth_shadow.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_depth_shadow_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_depth_shadow/sm_depth_shadow.ps.hlsl", "sm_depth_shadow.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::smsky_vertex:
            describe(description, key, e_shader_stage::vertex,
                "smsky/smsky.vs.hlsl", "smsky.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::smsky_daylight_pixel:
            describe(description, key, e_shader_stage::pixel,
                "smsky/smsky_daylight.ps.hlsl", "smsky_daylight.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::smsky_night_pixel:
            describe(description, key, e_shader_stage::pixel,
                "smsky/smsky_night.ps.hlsl", "smsky_night.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::smsky_combined_pixel:
            describe(description, key, e_shader_stage::pixel,
                "smsky/smsky_combined.ps.hlsl", "smsky_combined.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::road_lights_vertex:
            describe(description, key, e_shader_stage::vertex,
                "road_lights/road_lights.vs.hlsl", "road_lights.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::road_lights_pixel:
            describe(description, key, e_shader_stage::pixel,
                "road_lights/road_lights.ps.hlsl", "road_lights.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::fake_peds_vertex:
            describe(description, key, e_shader_stage::vertex,
                "fake_peds/fake_peds.vs.hlsl", "fake_peds.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::fake_peds_pixel:
            describe(description, key, e_shader_stage::pixel,
                "fake_peds/fake_peds.ps.hlsl", "fake_peds.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_citylod_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_citylod/sm_citylod.vs.hlsl", "sm_citylod.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_citylod_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_citylod/sm_citylod.ps.hlsl", "sm_citylod.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_retrofit_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_retrofit/sm_retrofit.vs.hlsl", "sm_retrofit.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_retrofit_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_retrofit/sm_retrofit.ps.hlsl", "sm_retrofit.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_buildinglod_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_buildinglod/sm_buildinglod.vs.hlsl", "sm_buildinglod.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_buildinglod_adaptive_alpha_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_buildinglod/sm_buildinglod.ps.hlsl",
                "sm_buildinglod.adaptive_alpha.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_buildinglod_opaque_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_buildinglod/sm_buildinglod.ps.hlsl",
                "sm_buildinglod.opaque.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = { "NGL_SM_BUILDINGLOD_OPAQUE", "1" };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_roadlod_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_roadlod/sm_roadlod.vs.hlsl", "sm_roadlod.vs.hlsl", "vs_3_0");
            
            return true;
        case e_shader_program::sm_roadlod_surface_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_roadlod/sm_roadlod.surface.ps.hlsl",
                "sm_roadlod.surface.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_roadlod_sampler_passthrough_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_roadlod/sm_roadlod.sampler_passthrough.ps.hlsl",
                "sm_roadlod.sampler_passthrough.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_road_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_road/sm_road.vs.hlsl",
                "sm_road.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_road_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_road/sm_road.material.ps.hlsl",
                "sm_road.material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_ROAD_SHADOW_COUNT",
                sm_road_shadow_definitions[key.permutation]
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_road_sample_auxiliary_alpha_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_road/sm_road.sample_auxiliary_alpha.ps.hlsl",
                "sm_road.sample_auxiliary_alpha.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_phatpalettecharnormal_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatpalettecharnormal/sm_phatpalettecharnormal.material.vs.hlsl",
                "sm_phatpalettecharnormal.material.vs.hlsl",
                "vs_3_0");

            description.definitions[0] = { "NGL_SM_CHARACTER_FORCE_TINT", "1" };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phatpalettecharnormal_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatpalettecharnormal/sm_phatpalettecharnormal.depth_shadow.vs.hlsl",
                "sm_phatpalettecharnormal.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phatpalettecharnormal_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette_normal/sm_phat_palette_normal.material.ps.hlsl",
                "sm_phatpalettecharnormal.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                {
                    "NGL_SM_PALETTE_NORMAL_SHADOW_COUNT",
                    sm_phat_palette_normal_shadow_definitions[key.permutation]
                },
                { "NGL_SM_PALETTE_NORMAL_BILINEAR_ONLY",    "1" },
                { "NGL_SM_PALETTE_NORMAL_FORCE_COLOR_LAST", "1" },
            }};
            description.definition_count = 3;
            
            return true;
        case e_shader_program::sm_phatpalettecharnormal_sample_auxiliary_alpha_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatpalettecharnormal/sm_phatpalettecharnormal.sample_auxiliary_alpha.ps.hlsl",
                "sm_phatpalettecharnormal.sample_auxiliary_alpha.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_phatpalettechar_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette_normal/sm_phat_palette_normal.material.ps.hlsl",
                "sm_phatpalettechar.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                {
                    "NGL_SM_PALETTE_NORMAL_SHADOW_COUNT",
                    sm_phat_palette_normal_shadow_definitions[key.permutation]
                },
                { "NGL_SM_PALETTE_HEIGHT_NORMAL", "1" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_bright_filter_vertex: {
            const sm_bright_filter::shader_configuration &configuration =
                sm_bright_filter::vertex_configurations[key.permutation];

            describe(description, key, e_shader_stage::vertex,
                configuration.source_path,
                configuration.source_name,
                "vs_3_0");

            description.definitions[0] = { configuration.definition, "1" };
            description.definition_count = 1;
            
            return true;
        }
        case e_shader_program::sm_bright_filter_pixel: {
            const sm_bright_filter::shader_configuration &configuration =
                sm_bright_filter::pixel_configurations[key.permutation];

            describe(description, key, e_shader_stage::pixel,
                configuration.source_path,
                configuration.source_name,
                "ps_3_0");

            description.definitions[0] = { configuration.definition, "1" };
            description.definition_count = 1;
            
            return true;
        }
        case e_shader_program::sm_phat_palette_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phat_palette/sm_phat_palette.material.vs.hlsl",
                "sm_phat_palette.material.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phat_palette_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette_normal/sm_phat_palette_normal.material.ps.hlsl",
                "sm_phat_palette.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                {
                    "NGL_SM_PALETTE_NORMAL_SHADOW_COUNT",
                    sm_phat_palette_normal_shadow_definitions[key.permutation]
                },
                { "NGL_SM_PALETTE_HEIGHT_NORMAL",        "1" },
                { "NGL_SM_PALETTE_STATIC_HEIGHT_NORMAL", "1" },
            }};
            description.definition_count = 3;
            
            return true;
        case e_shader_program::sm_phat_palette_sample_texture_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette/sm_phat_palette.sample_texture.ps.hlsl",
                "sm_phat_palette.sample_texture.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_phatcharnormal_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatpalettecharnormal/sm_phatpalettecharnormal.material.vs.hlsl",
                "sm_phatcharnormal.material.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phatcharnormal_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_babyphatnormal/sm_babyphatnormal.material.ps.hlsl",
                "sm_phatcharnormal.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                {
                    "NGL_SM_BABYPHATNORMAL_SHADOW_COUNT",
                    sm_babyphatnormal_shadow_definitions[key.permutation]
                },
                { "NGL_SM_BABYPHATNORMAL_FORCE_COLOR", "1" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_phatcharnormal_sample_texture_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatcharnormal/sm_phatcharnormal.sample.ps.hlsl",
                "sm_phatcharnormal.sample_texture.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PHATCHARNORMAL_SAMPLE_TEXTURE",
                "1"
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phatcharnormal_sample_green_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatcharnormal/sm_phatcharnormal.sample.ps.hlsl",
                "sm_phatcharnormal.sample_green.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PHATCHARNORMAL_SAMPLE_GREEN",
                "1"
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phatchar_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_babyphat/sm_babyphat.material.ps.hlsl",
                "sm_phatchar.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                {
                    "NGL_SM_BABYPHAT_SHADOW_COUNT",
                    sm_babyphat_shadow_definitions[key.permutation]
                },
                { "NGL_SM_BABYPHAT_FORCE_COLOR", "1" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_phatspiderman_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatspiderman/sm_phatspiderman.material.vs.hlsl",
                "sm_phatspiderman.material.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phatspiderman_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatspiderman/sm_phatspiderman.depth_shadow.vs.hlsl",
                "sm_phatspiderman.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phatspiderman_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatspiderman/sm_phatspiderman.material.ps.hlsl",
                "sm_phatspiderman.material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PHATSPIDERMAN_SHADOW_COUNT",
                sm_phatspiderman_shadow_definitions[key.permutation]
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phatspiderman_sample_texture_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatspiderman/sm_phatspiderman.sample.ps.hlsl",
                "sm_phatspiderman.sample_texture.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PHATSPIDERMAN_SAMPLE_TEXTURE",
                "1"
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phatspiderman_sample_green_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phatspiderman/sm_phatspiderman.sample.ps.hlsl",
                "sm_phatspiderman.sample_green.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PHATSPIDERMAN_SAMPLE_GREEN",
                "1"
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_decalchar_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_decalchar/sm_decalchar.material.vs.hlsl",
                "sm_decalchar.material.vs.hlsl",
                "vs_3_0");
            return true;
        case e_shader_program::sm_decalchar_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_decalchar/sm_decalchar.depth_shadow.vs.hlsl",
                "sm_decalchar.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_decalchar_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_decalchar/sm_decalchar.ps.hlsl", "sm_decalchar.ps.hlsl", "ps_3_0");
            
            return true;
        case e_shader_program::sm_bush_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_bush/sm_bush.material.vs.hlsl",
                "sm_bush.material.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_bush_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_bush/sm_bush.material.ps.hlsl",
                "sm_bush.material.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_bush_shadowed_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_bush/sm_bush.material.ps.hlsl",
                "sm_bush.shadowed_material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = { "NGL_SM_BUSH_SHADOWED", "1" };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_bush_constant_red_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_bush/sm_bush.constant_red.ps.hlsl",
                "sm_bush.constant_red.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_bush_sample_texture_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_bush/sm_bush.sample_texture.ps.hlsl",
                "sm_bush.sample_texture.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_bush_sample_auxiliary_texture_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_bush/sm_bush.sample_texture.ps.hlsl",
                "sm_bush.sample_auxiliary_texture.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = { "NGL_SM_BUSH_AUXILIARY_COORDINATES", "1" };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_babyphatnormal_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_babyphat/sm_babyphat.material.vs.hlsl",
                "sm_babyphatnormal.material.vs.hlsl",
                "vs_3_0");

            description.definitions = {{
                { "NGL_SM_VERTEX_COLOR",           "1" },
                { "NGL_SM_CONSTANT_AMBIENT_ALPHA", "0" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_babyphatnormal_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_babyphatnormal/sm_babyphatnormal.depth_shadow.vs.hlsl",
                "sm_babyphatnormal.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_babyphatnormal_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_babyphatnormal/sm_babyphatnormal.material.ps.hlsl",
                "sm_babyphatnormal.material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_BABYPHATNORMAL_SHADOW_COUNT",
                sm_babyphatnormal_shadow_definitions[key.permutation]
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_babyphat_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_babyphat/sm_babyphat.material.vs.hlsl",
                "sm_babyphat.material.vs.hlsl",
                "vs_3_0");

            description.definitions = {{
                { "NGL_SM_VERTEX_COLOR",           "0" },
                { "NGL_SM_CONSTANT_AMBIENT_ALPHA", "0" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_babyphat_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_babyphat/sm_babyphat.depth_shadow.vs.hlsl",
                "sm_babyphat.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_babyphat_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_babyphat/sm_babyphat.material.ps.hlsl",
                "sm_babyphat.material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_BABYPHAT_SHADOW_COUNT",
                sm_babyphat_shadow_definitions[key.permutation]
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_decal_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_decal/sm_decal.vs.hlsl",
                "sm_decal.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_decal_material_pixel: {
            const sm_decal::material_configuration &configuration =
                sm_decal::material_configurations[key.permutation];

            describe(description, key, e_shader_stage::pixel,
                "sm_decal/sm_decal.material.ps.hlsl",
                "sm_decal.material.ps.hlsl",
                "ps_3_0");

            description.definitions = {{
                { "NGL_SM_DECAL_HORIZON",     definition(configuration.horizon) },
                { "NGL_SM_DECAL_GOBO",        definition(configuration.gobo) },
                { "NGL_SM_DECAL_ENVIRONMENT", definition(configuration.environment) },
            }};
            description.definition_count = 3;
            
            return true;
        }
        case e_shader_program::sm_decal_facing_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_decal/sm_decal.facing.ps.hlsl",
                "sm_decal.facing.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_decal_ambient_alpha_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_decal/sm_decal.ambient_alpha.ps.hlsl",
                "sm_decal.ambient_alpha.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_phat_palette_normal_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phat_palette_normal/sm_phat_palette_normal.material.vs.hlsl",
                "sm_phat_palette_normal.material.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phat_palette_normal_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phat_palette_normal/sm_phat_palette_normal.depth_shadow.vs.hlsl",
                "sm_phat_palette_normal.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phat_palette_normal_material_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette_normal/sm_phat_palette_normal.material.ps.hlsl",
                "sm_phat_palette_normal.material.ps.hlsl",
                "ps_3_0");

            description.definitions[0] = {
                "NGL_SM_PALETTE_NORMAL_SHADOW_COUNT",
                sm_phat_palette_normal_shadow_definitions[key.permutation]
            };
            description.definition_count = 1;
            
            return true;
        case e_shader_program::sm_phat_palette_normal_sampler_passthrough_pixel:
            describe(description, key, e_shader_stage::pixel,
                "sm_phat_palette_normal/sm_phat_palette_normal.sampler_passthrough.ps.hlsl",
                "sm_phat_palette_normal.sampler_passthrough.ps.hlsl",
                "ps_3_0");
            
            return true;
        case e_shader_program::sm_phatnormal_material_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_babyphat/sm_babyphat.material.vs.hlsl",
                "sm_phatnormal.material.vs.hlsl",
                "vs_3_0");

            description.definitions = {{
                { "NGL_SM_VERTEX_COLOR",           "0" },
                { "NGL_SM_CONSTANT_AMBIENT_ALPHA", "1" },
            }};
            description.definition_count = 2;
            
            return true;
        case e_shader_program::sm_phatnormal_depth_shadow_vertex:
            describe(description, key, e_shader_stage::vertex,
                "sm_phatnormal/sm_phatnormal.depth_shadow.vs.hlsl",
                "sm_phatnormal.depth_shadow.vs.hlsl",
                "vs_3_0");
            
            return true;
        case e_shader_program::sm_phat_material_vertex:
        case e_shader_program::sm_phat_material_pixel:
        case e_shader_program::sm_phat_debug_pixel:
            return describe_sm_phat(key, description);
        case e_shader_program::scene_color_copy_vertex:
            describe(description, key, e_shader_stage::vertex,
                "scene_color/scene_color.copy.vs.hlsl",
                "scene_color.copy.vs.hlsl",
                "vs_3_0");

            return true;
        case e_shader_program::scene_color_copy_pixel:
            describe(description, key, e_shader_stage::pixel,
                "scene_color/scene_color.copy.ps.hlsl",
                "scene_color.copy.ps.hlsl",
                "ps_3_0");

            return true;
        case e_shader_program::scene_color_sanitize_pixel:
            describe(description, key, e_shader_stage::pixel,
                "scene_color/scene_color.sanitize.ps.hlsl",
                "scene_color.sanitize.ps.hlsl",
                "ps_3_0");

            return true;
        case e_shader_program::count:
            return false;
    }

    return false;
}
