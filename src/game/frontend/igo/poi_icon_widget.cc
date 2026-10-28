#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/poi_icon_widget.hh"

using namespace treyarch;

// sub_68C4B0
void poi_icon_widget::draw() {
    if (!is_shown || !unk_3c)
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

// inlined @ sub_6F4440
poi_icon_widget::~poi_icon_widget() {
    // slot 0
    if (unk_24) {
        ((void (__thiscall*)(igo_3d_drawable*, u32))unk_24->vtable->reserved_000[0])(unk_24, 1);
        unk_24 = nullptr;
    }

    // slot 0
    if (unk_2c) {
        ((void (__thiscall*)(igo_3d_drawable*, u32))unk_2c->vtable->reserved_000[0])(unk_2c, 1);
        unk_2c = nullptr;
    }

    // slot 0
    if (unk_28) {
        ((void (__thiscall*)(igo_3d_drawable*, u32))unk_28->vtable->reserved_000[0])(unk_28, 1);
        unk_28 = nullptr;
    }
}
