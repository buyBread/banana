#include <cmath>
#include <cstring>

#include "retail.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/shared/math/types/vector4.hh"

using namespace treyarch;

bool wds_render_manager::is_special_case_level() {
    cutscene_player* player = treyarch::references::cutscene_player.read();

    const char* level_name = (const char*)retail::sub_97E390((u32*)references::game_state.read());

    return level_name && !player->is_playing() &&
           (!std::strncmp(level_name, "act4_h", 6) || !std::strcmp(level_name, "Act3_Vulture_Hive"));
}

void wds_render_manager::publish_scene_parameter(ngl::scene* value) {
    u32 parameter_id    = ngl::fx::references::parameter_id_scene_light_source.read();
    u32 parameter_value = retail::sub_7D72F0();

    u64* valid_parameters = (u64*)value->parameters;

    *valid_parameters |= (u64)1 << parameter_id;

    value->parameters->values[parameter_id] = parameter_value;
}

f32 wds_render_manager::calculate_scene_near_plane(world_dynamics_system* world) {
    cutscene_player* player = treyarch::references::cutscene_player.read();
    f32 result = 0.25f;

    if (!player->is_playing()) {
        vector4 difference = world->hero_ptr->my_abs_po->matrix.w;

        difference.w = 0.0f;

        difference -= references::near_plane_reference.get();

        f64 squared_length = (f64)difference.x * (f64)difference.x +
                             (f64)difference.y * (f64)difference.y +
                             (f64)difference.z * (f64)difference.z +
                                 (f64)difference.w * (f64)difference.w;
        f32 adjusted_length = (f32)std::sqrt((f64)(f32)squared_length) - 2.0f;

        if (adjusted_length < 0.3f)
            adjusted_length = 0.3f;
        else if (adjusted_length > 0.5f)
            adjusted_length = 0.5f;

        result = adjusted_length;
    } else {
        u8* active_cutscene = *(u8**)player;

        if (active_cutscene)
            result = *(f32*)(active_cutscene + 0x124);
    }

    return result;
}

void wds_render_manager::configure_game_scene() {
    ngl::scene_callback_function null_callback =
        (ngl::scene_callback_function)retail::sub_5B4A70;

    ngl::set_scene_callback(ngl::scene_callback_mid,  null_callback);
    ngl::set_scene_callback(ngl::scene_callback_post, null_callback);
    ngl::set_scene_callback(ngl::scene_callback_3,    null_callback);

    ngl::set_scene_option_group_0(false, true, false);
    ngl::set_scene_option_group_1(false, true, ngl::d3d9::references::particle_depth_texture_requested.read());

    retail::sub_75E7D0();

    ngl::set_scene_callback(ngl::scene_callback_4,
                            (ngl::scene_callback_function)retail::sub_95CEF0);
}

void wds_render_manager::render_z_pre_pass(ngl::scene* game_scene, f32 near_plane) {
    ngl::scene* z_pre_pass = ngl::list_begin_scene(ngl::scene_parameter_parent);

    references::z_pre_pass_scene.write(z_pre_pass);

    ngl::set_scene_name("Z pre pass");
    ngl::set_clear_flags(0);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_color_target(nullptr);

    z_pre_pass->depth_bias_enabled     = true;
    z_pre_pass->near_plane             = near_plane - 0.19f;
    z_pre_pass->derived_matrices_dirty = true;

    ngl::validate_matrices(z_pre_pass);

    ngl::list_end_scene();

    if (references::z_pre_pass_enabled.read()) {
        ngl::scene* previous = ngl::list_select_scene(z_pre_pass);

        retail::sub_972EF0(1);

        ngl::list_select_scene(previous);
    }

    ngl::list_select_scene(game_scene);

    game_scene->near_plane             = near_plane;
    game_scene->derived_matrices_dirty = true;

    ngl::validate_matrices(game_scene);
}
