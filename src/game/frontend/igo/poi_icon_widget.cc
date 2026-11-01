#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/poi_icon_widget.hh"

using namespace treyarch;

// sub_68C3F0
void poi_icon_widget::turn_on(bool on) {
    if (is_shown == on)
        return;

    is_shown = on;

    if (unk_24)
        unk_24->set_shown(on);

    unk_2c->set_shown(on);
    unk_3c = on;
}

// sub_68C4B0
void poi_icon_widget::draw() {
    if (!is_shown || !unk_3c)
        return;

    ui_frontend* igo = references::frontend.get().igo;

    if (igo->letterbox_widget && igo->letterbox_widget->is_shown())
        return;

    if (igo->zoom_map->is_open_or_zooming())
        return;

    if (state - 1 <= 1 && unk_24)
        unk_24->render();

    if (unk_2c)
        unk_2c->render();

    if (unk_28)
        unk_28->render();
}

// inlined @ sub_6F4440
poi_icon_widget::~poi_icon_widget() {
    if (unk_24) {
        delete unk_24;
        unk_24 = nullptr;
    }

    if (unk_2c) {
        delete unk_2c;
        unk_2c = nullptr;
    }

    if (unk_28) {
        delete unk_28;
        unk_28 = nullptr;
    }
}
