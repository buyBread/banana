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
#include "treyarch/soap/soap.hh"
#include "treyarch/soap/storage.hh"

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

// sub_6BC560
void igo_3d_scrapbook::unk_002(i32 unk) {
    if (is_background_fading())
        return;

    igo_3d_scrapbook_station_widget* station = current_station();

    if (station && station->unk_082() && unk_0c4 > 0.5f)
        unk_028();
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

// sub_6BC870
void igo_3d_scrapbook::unk_010(i32 unk) {
    igo_3d_scrapbook_station_widget* station = current_station();

    if (unk_09d) {
        if (!is_background_fading() && unk_0c4 > 0.5f)
            unk_028();

        return;
    }

    if (station)
        station->page_on_cancel(unk);
}

// sub_6BC8F0
void igo_3d_scrapbook::unk_011(i32 unk) {
    igo_3d_scrapbook_station_widget* station = current_station();

    if (station && !station->has_open_dialog())
        station->select_previous(unk);
}

// sub_6BC940
void igo_3d_scrapbook::unk_012(i32 unk) {
    igo_3d_scrapbook_station_widget* station = current_station();

    if (station && !station->has_open_dialog())
        station->select_next(unk);
}

// slot 13
void igo_3d_scrapbook::unk_013(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1BB0)(this, unk);
}

// slot 14
void igo_3d_scrapbook::unk_014(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1D40)(this, unk);
}

// sub_6BC5D0
void igo_3d_scrapbook::unk_015(i32 unk) {
    igo_3d_scrapbook_station_widget* station = current_station();

    if (station && station->unk_078())
        station->unk_071(unk);
    else
        unk_003(unk);
}

// sub_6BC680
void igo_3d_scrapbook::unk_016(i32 unk) {
    igo_3d_scrapbook_station_widget* station = current_station();

    if (station && station->unk_078())
        station->unk_072(unk);
    else
        unk_004(unk);
}

// slot 24
void igo_3d_scrapbook::unk_024() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_6FB430)(this);
}

// slot 25
void igo_3d_scrapbook::unk_025(f32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, f32))retail::sub_6D1170)(this, unk);
}

// sub_68FF10
void igo_3d_scrapbook::draw_pages() {
    if (!active)
        return;

    if (tab_pages)
        tab_pages->render();

    if (sub_tab_pages)
        sub_tab_pages->render();

    if (main_panel)
        main_panel->render();

    if (pause_bar)
        pause_bar->render();
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
            stations[index]->put_away();
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

    set_page(0, false, 0);

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

// sub_68FF90
bool igo_3d_scrapbook::can_open() {
    ui_frontend* frontend = references::frontend.get().igo;

    if (frontend->zoom_map && frontend->zoom_map->is_zooming)
        return false;

    if (frontend->modal_save_dialog && frontend->modal_save_dialog->is_shown())
        return false;

    if (frontend->generic_modal_dialog && frontend->generic_modal_dialog->is_shown())
        return false;

    if (soap::storage::inst()->busy() == 1 && soap::references::enable_storage.read())
        return false;

    game* the_game = references::game.read();

    if (the_game && the_game->data && retail::sub_77CA40((u32*)the_game->data))
        return false;

    return !sliding;
}

// inlined @ sub_6BC560
// inlined @ sub_6BC5D0
// inlined @ sub_6BC680
// inlined @ sub_6BC870
// inlined @ sub_6BC8F0
// inlined @ sub_6BC940
igo_3d_scrapbook_station_widget* igo_3d_scrapbook::current_station() {
    if (current_page < 0 || current_page >= (i32)stations.size())
        return nullptr;

    return stations[current_page];
}

// inlined @ sub_6BC560
// inlined @ sub_6BC870
bool igo_3d_scrapbook::is_background_fading() {
    u32 mode = references::frontend.get().igo->background_effect_mode;

    return mode == 2 || mode == 3;
}

// sub_6BC990
void igo_3d_scrapbook::set_page(i32 request, bool animate, i32 unlock) {
    i32 page     = first_unlocked_page(request, unlock);
    i32 old_page = current_page;

    if (page == old_page)
        return;

    if (old_page >= 0 && old_page < (i32)stations.size() && stations[old_page])
        stations[old_page]->put_away();

    current_page = page;

    retail::sub_9EB5D0((u32*)soap::online::inst(), 4, current_page); // online::set_context

    soap::online::inst()->method_024(1);

    if (current_page >= 0 && current_page < (i32)stations.size() && stations[current_page])
        stations[current_page]->bring_up();

    // the pages sit in a strip, the last one at main_panel_x
    f32 from = (f32)(stations.size() - old_page - 1) * -803.0f + main_panel_x + 0.0f;
    f32 to   = (f32)(stations.size() - page - 1)     * -803.0f + main_panel_x + 0.0f;

    if (animate) {
        slide_elapsed = 0.0f;
        slide_from    = from;
        sliding       = true;
        slide_to      = to;
    } else if (main_panel) {
        f32 z = main_panel->get_position_z();
        f32 y = main_panel->get_position_y();

        main_panel->set_position(to, y, z);
    }

    if (tab_pages)
        tab_pages->set_current_tab(current_page);
}

// sub_6A5290
i32 igo_3d_scrapbook::first_unlocked_page(i32 request, i32 unlock) {
    if (request == 0) {
        if (stations[0]->is_unlocked(unlock))
            return 0;
    } else if (request != 1)
        return 2;

    return stations[1]->is_unlocked(unlock) ? 1 : 2;
}

// sub_6A5570
void igo_3d_scrapbook::clear_page_unlocks() {
    for (u32 index = 0; index < stations.size(); ++index) {
        if (stations[index])
            stations[index]->page_unlocked = false;
    }
}

// sub_6A55B0
bool igo_3d_scrapbook::is_any_page_locked() {
    bool locked = false;

    for (i32 index = 0; index < 3; ++index)
        locked = locked || !stations[index]->is_unlocked(false);

    return locked;
}
