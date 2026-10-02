#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/frontend/ui_object_manager.hh"

using namespace treyarch;

// sub_68C4B0
void ui_object_display_1c::draw() {
    if (!enabled || !unk_3c)
        return;

    ui_frontend* igo = references::frontend.get().igo;

    if (igo->letterbox && igo->letterbox->is_visible())
        return;

    if (igo->zoom_map->blocks_world_rendering())
        return;

    if (state - 1 <= 1 && unk_24)
        unk_24->draw();

    if (unk_2c)
        unk_2c->draw();

    if (unk_28)
        unk_28->draw();
}

// sub_68FC00
void ui_object_display_30::draw() {
    if (!enabled)
        return;

    if (references::frontend.get().igo->zoom_map->blocks_world_rendering())
        return;

    if (unk_08)
        unk_08->draw();

    if (state - 1 <= 1 && unk_0c)
        unk_0c->draw();
}

// sub_6D2710
void ui_object_manager::draw() {
    bool background_mode_4 = references::frontend.get().igo->background_effect_mode == 4;

    for (auto* position = keys.begin(); position != keys.end(); position = position->next()) {
        u32        key    = position->value.second;
        ui_object* object = nullptr;
        bool       found;

        {
            engine_lock_scope scope(&objects.lock);

            ui_object_table::entry &entry = objects.entries[key & objects.mask];

            found = entry.key == key;

            if (found)
                object = entry.object;
        }

        if (!found)
            continue;

        bool shown = background_mode_4 ? object->shown_in_mode_4 : object->shown;

        if (object->owner.resolve() && object->display_1c && object->display_1c->enabled && shown &&
            (!unk_7c || object->unk_4d)) {

            object->display_1c->draw();
        } else if (object->owner.resolve()) {
            if (object->display_30 && object->display_30->enabled)
                object->display_30->draw();
        }
    }
}
