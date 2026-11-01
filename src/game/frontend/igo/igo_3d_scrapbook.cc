#include "retail.hh"
#include "game/app.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_nav_button_bar.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/soap/online.hh"

using namespace treyarch;

// sub_6ED680
igo_3d_scrapbook::~igo_3d_scrapbook() {
    for (u32 index = 0; index < stations.size(); ++index) {
        if (stations[index]) {
            delete stations[index];
            stations[index] = nullptr;
        }
    }

    stations.clear();

    if (tab_pages) {
        delete tab_pages;
        tab_pages = nullptr;
    }

    if (sub_tab_pages) {
        delete sub_tab_pages;
        sub_tab_pages = nullptr;
    }

    if (pause_bar_text) {
        pause_bar_text->destroy();
        pause_bar_text = nullptr;
    }

    if (pause_bar) {
        delete pause_bar;
        pause_bar = nullptr;
    }
}

// slot 1
void igo_3d_scrapbook::unk_001(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_7160E0)(this, unk);
}

// slot 2
void igo_3d_scrapbook::unk_002(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC560)(this, unk);
}

// slot 3
void igo_3d_scrapbook::unk_003(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC640)(this, unk);
}

// slot 4
void igo_3d_scrapbook::unk_004(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC6F0)(this, unk);
}

// slot 5
void igo_3d_scrapbook::unk_005(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC730)(this, unk);
}

// slot 6
void igo_3d_scrapbook::unk_006(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC770)(this, unk);
}

// slot 7
void igo_3d_scrapbook::unk_007(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC7B0)(this, unk);
}

// slot 8
void igo_3d_scrapbook::unk_008(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC7F0)(this, unk);
}

// slot 9
void igo_3d_scrapbook::unk_009(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC830)(this, unk);
}

// slot 10
void igo_3d_scrapbook::unk_010(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC870)(this, unk);
}

// slot 11
void igo_3d_scrapbook::unk_011(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC8F0)(this, unk);
}

// slot 12
void igo_3d_scrapbook::unk_012(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC940)(this, unk);
}

// slot 13
void igo_3d_scrapbook::unk_013(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1BB0)(this, unk);
}

// slot 14
void igo_3d_scrapbook::unk_014(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1D40)(this, unk);
}

// slot 15
void igo_3d_scrapbook::unk_015(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC5D0)(this, unk);
}

// slot 16
void igo_3d_scrapbook::unk_016(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC680)(this, unk);
}

// slot 24
void igo_3d_scrapbook::unk_024() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_6FB430)(this);
}

// slot 25
void igo_3d_scrapbook::unk_025(f32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, f32))retail::sub_6D1170)(this, unk);
}

// slot 26
void igo_3d_scrapbook::draw_pages() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_68FF10)(this);
}

// sub_6D15D0
void igo_3d_scrapbook::activate(bool unk) {
    if (references::frontend.get().igo->zoom_map->is_open_or_zooming())
        return;

    unk_064.start(0.0f, 1.0f, 0.25f, 0.0f);

    unk_09c = true;

    string_hash activate_sound;
    activate_sound.initialize(mash::ALLOCATED, "scrapbook_activate");

    retail::sub_900D60(activate_sound.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    event_manager::raise_event(references::scrapbook_entered.read(), arch_base_vhandle());

    active = true;

    loading_screen_was_shown = references::frontend.get().igo->loading_screen->is_shown();

    if (loading_screen_was_shown) {
        igo_3d_loading_screen* loading_screen = references::frontend.get().igo->loading_screen;

        loading_screen->deferred_shown = loading_screen->is_shown();
        loading_screen->set_shown(false);

        // the count stays; unk_028 freezes the hero as many times again
        for (i32 count = loading_screen->hero_freezes; count > 0; --count)
            references::game.read()->freeze_hero(false);

        loading_screen->defer_set_shown = true;
    }

    game_was_paused = references::game.read()->game_paused;

    if (!game_was_paused)
        retail::sub_97C970((i32)references::game.read()); // game::pause_action

    retail::sub_6A5420(0.0f); // the camera widget's reset

    references::frontend.get().igo->zoom_map->toggle_open();

    for (u32 index = 0; index < stations.size(); ++index) {
        if (stations[index]) {
            stations[index]->unk_054();
            stations[index]->unk_057();
        }
    }

    if (tab_pages) {
        tab_pages->first_visible_tab = 0;
        tab_pages->set_current_tab(0);
        tab_pages->slide_in();
    }

    if (sub_tab_pages) {
        sub_tab_pages->set_shown(true);
        sub_tab_pages->set_position(sub_tab_pages->initial_position.x,
                                    sub_tab_pages->initial_position.y,
                                    sub_tab_pages->initial_position.z);
    }

    current_page = -1;

    retail::sub_6BC990((u32*)this, 0, 0, 0); // switches to the first unlocked page

    unk_09d = unk;

    string_hash map_sound;
    map_sound.initialize(mash::ALLOCATED, "ui_map_turn_on");

    retail::sub_900D60(map_sound.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    unk_0c4 = 0.0f;
}

// sub_6D1930
void igo_3d_scrapbook::unk_028() {
    active  = false;
    unk_09d = false;

    retail::sub_6A5420(0.0f); // the camera widget's reset

    if (loading_screen_was_shown) {
        igo_3d_loading_screen* loading_screen = references::frontend.get().igo->loading_screen;

        loading_screen->defer_set_shown = false;
        loading_screen->set_shown(loading_screen->deferred_shown);

        for (i32 count = loading_screen->hero_freezes; count > 0; --count)
            references::game.read()->freeze_hero(true);

        loading_screen->defer_set_shown = false;
    }

    unk_04 |= 2;

    for (u32 index = 0; index < stations.size(); ++index) {
        if (stations[index])
            stations[index]->unk_055();
    }

    if (references::frontend.get().igo->nav_button_bar) {
        references::frontend.get().igo->nav_button_bar->unk_007();
        references::frontend.get().igo->nav_button_bar->unk_001(false);

        unk_038 = -1;
    }

    if (!game_was_paused)
        retail::sub_97BB70((i32)references::game.read()); // game::unpause_action

    event_manager::raise_event(references::scrapbook_exited.read(), arch_base_vhandle());

    if (tab_pages)
        tab_pages->slide_out();

    soap::online::inst();

    i32 presence;

    if (mission_manager::inst()->is_mission_running()) {
        i32 context = retail::sub_7342C0(1, mission_manager::inst()->get_running_script_index());

        if (context != -1) {
            soap::online* online = soap::online::inst();

            retail::sub_9EB5D0((u32*)online, 2, mission_manager::inst()->get_current_act()); // online::set_context
            retail::sub_9EB5D0((u32*)soap::online::inst(), 5, context);

            presence = 2;
        } else {
            retail::sub_9EB5D0((u32*)soap::online::inst(), 1, 3); // online::set_context

            presence = 0;
        }
    } else {
        retail::sub_9EB5D0((u32*)soap::online::inst(), 1, 4); // online::set_context

        presence = 0;
    }

    soap::online::inst()->method_024(presence);

    references::frontend.get().igo->zoom_map->toggle_open();

    string_hash map_sound;
    map_sound.initialize(mash::ALLOCATED, "ui_map_turn_off");

    retail::sub_900D60(map_sound.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    unk_09e = false;
}

// slot 29
u8 igo_3d_scrapbook::is_active() {
    return ((u8 (__thiscall*)(igo_3d_scrapbook*))retail::sub_6ED7C0)(this);
}

// slot 30
bool igo_3d_scrapbook::unk_030() {
    return ((bool (__thiscall*)(igo_3d_scrapbook*))retail::sub_690030)(this);
}

// slot 31
bool igo_3d_scrapbook::unk_031() {
    return ((bool (__thiscall*)(igo_3d_scrapbook*))retail::sub_68FF90)(this);
}
