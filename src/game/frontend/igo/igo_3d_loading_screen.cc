#include <cmath>

#include <windows.h>

#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/ise_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_6FC080
igo_3d_loading_screen::~igo_3d_loading_screen() {
    release_entity();

    if (message_text) {
        message_text->destroy();
        message_text = nullptr;
    }
}

// slot 1
void igo_3d_loading_screen::init() {
    ((void (__thiscall*)(igo_3d_loading_screen*))retail::sub_691FC0)(this);
}

// sub_6C5D90
void igo_3d_loading_screen::update(f32 time_inc) {
    if (is_shown() && fade_duration > 0.0f) {
        fade_elapsed = (f32)((f64)fade_elapsed + (f64)time_inc);

        if ((f64)fade_elapsed < (f64)fade_duration) {
            f64 progress = (f64)fade_elapsed / (f64)fade_duration;

            if (fading_in) {
                f32 target = (f32)progress;
                f32 value  = target > fade ? target : fade;

                fade = 1.0f > value ? value : 1.0f;
            } else {
                f32 target = (f32)(1.0 - progress);
                f32 value  = fade > target ? target : fade;

                fade = (f64)value > 0.0 ? value : 0.0f;
            }
        } else {
            if (fading_in) {
                fade = 1.0f;

                if (!no_hero_freeze) {
                    references::game.read()->freeze_hero(true);
                    ++hero_freezes;
                }

                entity* hero = references::g_world_ptr.read()->get_hero_ptr();

                if (hero)
                    hero->ise_ifc()->unk_020();
            } else {
                entity* hero = references::g_world_ptr.read()->get_hero_ptr();

                if (hero && hero->has_player_ifc())
                    hero->player_ifc()->unk_4b8 = 0.0f;

                fade = 0.0f;

                set_shown(false);
            }

            fade_duration = 0.0f;
            fade_elapsed  = 0.0f;
            fading_in     = false;
        }

        // holds a frame to at least 0.05 seconds while fading
        if (time_inc < 0.05f)
            Sleep((DWORD)(i64)((0.05f - time_inc) * 1000.0f));
    }

    if (is_shown() && fade > 0.9f) {
        f32 cycle = unk_150;

        if (cycle > 4.0f)
            muted = true;

        cycle   = (f32)std::fmod((f64)(cycle + time_inc), 4.0);
        unk_150 = cycle;
        unk_180 = cycle * 4.71f;

        opaque_time = (f32)((f64)opaque_time + (f64)time_inc);

        if (opaque_time >= opaque_timeout) {
            opaque_time = 0.0f;

            blackscreen_off(1.0f, true);
        }

        mission_manager* missions = mission_manager::inst();

        if ((!unk_166 || held) &&
            unk_150 >= 3.0f &&
            !missions->is_mission_running() &&
            !missions->is_mission_ready() &&
            !missions->is_maloring() &&
            !retail::sub_97E250((u32*)missions) &&
            !missions->has_auto_launch_instances() &&
            missions->get_state() &&
            retail::sub_902810() && // is_bank_loader_idle
            retail::sub_77BF80())

            blackscreen_off(1.0f, held);
    }
}

// slot 3
void igo_3d_loading_screen::render() {
    ((void (__thiscall*)(igo_3d_loading_screen*))retail::sub_691FE0)(this);
}

// slot 5
void igo_3d_loading_screen::set_shown(bool shown) {
    ((void (__thiscall*)(igo_3d_loading_screen*, bool))retail::sub_6A9EB0)(this, shown);
}

// slot 42
void igo_3d_loading_screen::spawn_entity() {
    ((void (__thiscall*)(igo_3d_loading_screen*))retail::sub_6EDC40)(this);
}

// slot 44
void igo_3d_loading_screen::render_congomerate_mesh(ngl::mesh*              mesh,
                                                    i32                     index,
                                                    const matrix4x4        &trans_matrix,
                                                    const matrix4x4        &rel_pos_matrix,
                                                    ngl::mesh_params*       mesh_params,
                                                    ngl::shader_param_set*  shader_params) {

    ((void (__thiscall*)(igo_3d_loading_screen*, ngl::mesh*, i32, const matrix4x4&, const matrix4x4&, ngl::mesh_params*, ngl::shader_param_set*))retail::sub_6A9F80)(this,
                                                                                                                                                                     mesh,
                                                                                                                                                                     index,
                                                                                                                                                                     trans_matrix,
                                                                                                                                                                     rel_pos_matrix,
                                                                                                                                                                     mesh_params,
                                                                                                                                                                     shader_params);
}

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
// inlined @ sub_6DE490
// inlined @ sub_6E70D0
// inlined @ sub_70A8B0
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
