#include "retail.hh"
#include "game/app.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_6D0DB0
// inlined @ sub_6DE490
bool igo_3d_dialog_widget::set_first_visible_line(i32 index) {
    if (index < 0)
        index = 0;
    else if ((u32)index >= lines.size())
        index = lines.size() - 1;

    bool changed = index != first_visible_line;

    first_visible_line = index;

    if (changed)
        retail::sub_6BA160((u32*)this); // shows the five lines from first_visible_line, hides the rest

    return changed;
}

// sub_6DE490
void igo_3d_dialog_widget::bring_up(i32 mode) {
    if (is_dialog_shown())
        return;

    references::frontend.get().igo->loading_screen->blackscreen_off(0.0f, false);

    event_manager::raise_event(references::dialog_3d_entered.read(), arch_base_vhandle());

    game_was_paused = references::game.read()->game_paused;

    unk_071(true);
    set_shown(true);

    bring_up_mode = mode;

    if (!game_was_paused)
        retail::sub_97C970((i32)references::game.read()); // game::pause_action

    unk_078();

    set_first_visible_line(0);

    retail::sub_6A22A0((u32*)this, false); // shows or hides every line

    unk_076(button_2_shown && !button_1_shown ? 2 : 1);

    if (button_1)
        button_1->set_shown(button_1_shown);

    if (button_2)
        button_2->set_shown(button_2_shown);

    fade_in(0.5f, 0.0f, 1.0f, 0.0f);

    unk_1e8.start(0.0f, 1.0f, 0.5f, 0.0f);
    unk_204.start(0.0f, 1.0f, 0.25f, 0.0f);

    retail::sub_6D8720((i32)references::frontend.get().igo, 1); // starts the dialog background fade

    string_hash objectives_on;
    objectives_on.initialize(mash::ALLOCATED, "ui_objectives_on");

    retail::sub_900D60(objectives_on.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    faded_in = true;
}
