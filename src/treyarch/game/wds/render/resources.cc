#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/shader_resource_manager.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

// inlined into sub_9772D0
void wds_render_manager::ensure_resources() {
    ngl::resources::resolve_effect(references::highlight_zprime_shader.get(), "highlight_zprime_shader");
    ngl::resources::resolve_effect(references::highlight_shader.get(),        "highlight_shader");
    ngl::resources::resolve_effect(references::highlight_skin_zprime.get(),   "highlight_skin_zprime_shader");
    ngl::resources::resolve_effect(references::highlight_skin.get(),          "highlight_skin_shader");
    ngl::resources::resolve_effect(references::buildinglod_zpass.get(),       "buildinglod_zpass");
    ngl::resources::resolve_effect(references::roadlod_foam_core.get(),       "roadlod_FoamCore");
    ngl::resources::resolve_effect(references::buildinglod_foam_core.get(),   "buildinglod_FoamCore");
    ngl::resources::resolve_effect(references::buildinglod_radar.get(),       "buildinglod_radar");

    if (!references::rvb_radar.read())
        references::rvb_radar.write(references::shader_resource_manager.read()->find_pixel_program("rvb_radar"));

    ngl::resources::resolve_texture(references::radar_stroke.get(),   "ui_hud_radar_stroke");
    ngl::resources::resolve_texture(references::dither_texture.get(), "dithertexture");
    ngl::resources::resolve_texture(references::horizon_clouds.get(), "horizon_clouds_day");
}

// inlined into sub_9772D0
void wds_render_manager::request_environment_texture() {
    if (ngl::fx::references::environment_texture.read())
        return;

    const char* empty_name = "";
    mission_manager* missions = references::mission_manager.read();

    __asm {
        push 0
        sub esp, 0Ch
        mov ecx, esp
        push empty_name
        call retail::sub_A6CE00
        mov ecx, missions
        call retail::sub_980760
    }
}