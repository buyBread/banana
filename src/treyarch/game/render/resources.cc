#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/render/resource_references.hh"
#include "treyarch/game/render/wds_render_manager.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"

namespace treyarch { namespace render_resources {
    void resolve_effect(ngl::fx::effect* &cache, const char* name) {
        if (cache)
            return;

        fixed_string resource_name = make_fixed_string(name);

        cache = (ngl::fx::effect*)ngl::resources::resolve(&resource_name, four_cc('F', 'X', '\0'));
    }

    void resolve_texture(ngl::texture* &cache, const char* name) {
        if (cache)
            return;

        fixed_string resource_name = make_fixed_string(name);

        cache = (ngl::texture*)ngl::resources::resolve(&resource_name, four_cc('T', 'E', 'X'));
    }
}} // treyarch::render_resources

namespace treyarch {
    void wds_render_manager::ensure_resources() {
        using namespace references;

        render_resources::resolve_effect(highlight_zprime_shader.get(), "highlight_zprime_shader");
        render_resources::resolve_effect(highlight_shader.get(),        "highlight_shader");
        render_resources::resolve_effect(highlight_skin_zprime.get(),   "highlight_skin_zprime_shader");
        render_resources::resolve_effect(highlight_skin.get(),          "highlight_skin_shader");
        render_resources::resolve_effect(buildinglod_zpass.get(),       "buildinglod_zpass");
        render_resources::resolve_effect(roadlod_foam_core.get(),       "roadlod_FoamCore");
        render_resources::resolve_effect(buildinglod_foam_core.get(),   "buildinglod_FoamCore");
        render_resources::resolve_effect(buildinglod_radar.get(),       "buildinglod_radar");

        if (!rvb_radar.read()) {
            rvb_radar.write(retail::sub_757C20((u32*)rvb_radar_resource_manager_maybe_idk.read(),
                                               (u8*)"rvb_radar"));
        }

        render_resources::resolve_texture(radar_stroke.get(),   "ui_hud_radar_stroke");
        render_resources::resolve_texture(dither_texture.get(), "dithertexture");
        render_resources::resolve_texture(horizon_clouds.get(), "horizon_clouds_day");
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
} // treyarch
