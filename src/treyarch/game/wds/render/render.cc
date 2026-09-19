#include "retail.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/wds/terrain.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

void wds_render_manager::render() {
    world_dynamics_system* world = get_world();

    retail::sub_96A350();

    cutscene_player* player = treyarch::references::cutscene_player.read();
    bool cutscene_is_playing = player->is_playing();
    u8* active_cutscene = cutscene_is_playing ? *(u8**)player : nullptr;

    references::render_distance.write
        (cutscene_is_playing && active_cutscene && !(*(u32*)(active_cutscene + 0x28) & 0x8000) ?
            2500.0f : 250.0f);

    references::frame_state_1117194.write(0);

    retail::sub_96E5B0((u32*)references::render_data.read());

    ensure_resources();
    request_environment_texture();

    references::level_render_special_case.write(0);

    if (is_special_case_level())
        references::level_render_special_case.write(1);

    ngl::scene* game_scene = ngl::references::current_scene.read();

    ngl::set_world_to_view_matrix(&references::world_to_view.get());

    ngl::calculate_matrices(game_scene);
    publish_scene_parameter(game_scene);

    retail::sub_9694A0((u64*)references::published_scene.read(), (i32)game_scene);

    if (!references::current_region.read()) {
        ngl::set_scene_callback(ngl::scene_callback_4,
                                (ngl::scene_callback_function)retail::sub_95CEF0);

        return;
    }

    references::render_lock.get().acquire();
    references::render_sequence.write(references::render_sequence.read() + 1);

    retail::sub_95D7D0((u32*)references::render_data.read(),
                        world->the_terrain->generation + 1);

    configure_game_scene();

    f32 near_plane = calculate_scene_near_plane(world);

    render_z_pre_pass(game_scene, near_plane);

    retail::sub_975C50();

    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    if (!zoom_map || !zoom_map->blocks_world_rendering())
        submit_world(world);
    else
        submit_blocked_world(world);

    references::render_lock.get().release();
}
