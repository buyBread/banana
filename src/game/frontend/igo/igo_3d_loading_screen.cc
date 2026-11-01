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

// slot 2
void igo_3d_loading_screen::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_loading_screen*, f32))retail::sub_6C5D90)(this, time_inc);
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
i32 igo_3d_loading_screen::unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_loading_screen*, i32, i32, void*, i32, i32, i32))retail::sub_6A9F80)(this,
                                                                                                           unk_0,
                                                                                                           unk_1,
                                                                                                           unk_2,
                                                                                                           unk_3,
                                                                                                           unk_4,
                                                                                                           unk_5);
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
