#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_707E90
void igo_3d_loading_screen::blackscreen_on(f32 time, bool unk, bool hold, bool silent, bool no_freeze, f32 timeout) {
    if (references::frontend.get().igo->controller_disconnect_dialog->is_shown())
        return;

    fading_in = true;

    if ((f64)time > 0.0001) {
        fade_duration = time;
        fade_elapsed  = 0.0f;

        if (!is_shown())
            fade = 0.0f;

        entity* hero = references::g_world_ptr.read()->get_hero_ptr();

        if (hero && hero->has_ifc(entity_ifc_player) && !no_freeze)
            hero->player_ifc()->unk_4b8 = time;
    } else {
        fade_duration = 0.0f;
        fade_elapsed  = 0.0f;
        fade          = 1.0f;
    }

    if (!is_shown()) {
        unk_150 = 0.0f;
        muted   = !silent;
        unk_166 = unk;
    }

    if (!silent) {
        string_hash mute;
        mute.initialize(mash::ALLOCATED, "ui_loading_mute");

        retail::sub_900D60(mute.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
    }

    if (hold)
        held = true;

    no_hero_freeze = no_freeze;

    if (!hint_picked) {
        retail::sub_6FFE10((i32)this); // picks the hint text
        hint_picked = true;
    }

    set_shown(true);

    opaque_timeout = timeout;
}

// sub_6A9D60
void igo_3d_loading_screen::blackscreen_off(f32 time, bool force) {
    if (!mission_manager::inst()->is_mission_running() && held && !force)
        return;

    fading_in = false;

    if ((f64)time > 0.0001) {
        fade_duration = time;
        fade_elapsed  = 0.0f;

        entity* hero = references::g_world_ptr.read()->get_hero_ptr();

        if (hero && hero->has_ifc(entity_ifc_player))
            hero->player_ifc()->unk_4b8 = time;
    } else {
        fade_duration = 0.0f;
        fade_elapsed  = 0.0f;
        fade          = 0.0f;

        set_shown(false);
    }

    if (!defer_set_shown) {
        string_hash unmute;
        unmute.initialize(mash::ALLOCATED, "ui_loading_unmute");

        retail::sub_900D60(unmute.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
    }

    while (hero_freezes > 0) {
        references::game.read()->freeze_hero(false);
        --hero_freezes;
    }

    held        = false;
    hint_picked = false;
}
