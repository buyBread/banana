#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/resources/references.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

void wds_render_manager::ensure_resources() {
    using namespace ngl::resources::references;

    ngl::resources::resolve_effect(highlight_zprime_shader.get(), "highlight_zprime_shader");
    ngl::resources::resolve_effect(highlight_shader.get(),        "highlight_shader");
    ngl::resources::resolve_effect(highlight_skin_zprime.get(),   "highlight_skin_zprime_shader");
    ngl::resources::resolve_effect(highlight_skin.get(),          "highlight_skin_shader");
    ngl::resources::resolve_effect(buildinglod_zpass.get(),       "buildinglod_zpass");
    ngl::resources::resolve_effect(roadlod_foam_core.get(),       "roadlod_FoamCore");
    ngl::resources::resolve_effect(buildinglod_foam_core.get(),   "buildinglod_FoamCore");
    ngl::resources::resolve_effect(buildinglod_radar.get(),       "buildinglod_radar");

    if (!rvb_radar.read()) {
        rvb_radar.write(retail::sub_757C20((u32*)shader_resource_manager.read(),
                                           (u8*)"rvb_radar"));
    }

    ngl::resources::resolve_texture(radar_stroke.get(),   "ui_hud_radar_stroke");
    ngl::resources::resolve_texture(dither_texture.get(), "dithertexture");
    ngl::resources::resolve_texture(horizon_clouds.get(), "horizon_clouds_day");
}

void wds_render_manager::request_environment_texture() {
    if (ngl::fx::references::environment_texture.read())
        return;

    const char* empty_name = "";
    u32* game_state_instance = (u32*)references::game_state.read();

    __asm {
        push 0
        sub esp, 0Ch
        mov ecx, esp
        push empty_name
        call retail::sub_A6CE00
        mov ecx, game_state_instance
        call retail::sub_980760
    }
}