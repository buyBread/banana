#include <bit>

#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/poi_icon_widget.hh"

using namespace treyarch;

// sub_70C4A0
poi_icon_widget::poi_icon_widget(i32 kind) {
    unk_1c      = 0;
    is_shown    = false;
    unk_1e      = 0;
    this->kind  = kind;
    mode        = 0;
    unk_1f      = 0;
    dot_visible = true;
    unk_18      = 0.9f;
    unk_24      = nullptr;

    i32   style      = widget_style(kind);
    void* allocation = mm::allocate(sizeof(igo_3d_poi_simple_widget));

    unk_2c = allocation ? (igo_3d_poi_simple_widget*)retail::sub_705F20((i32)allocation, style) : nullptr;

    if (unk_24)
        unk_24->init();

    if (unk_2c)
        unk_2c->init();

    u32 argb = 0xFFFFFFFF;

    switch (this->kind) {
        case 0:
            argb = 0xFF3CB43C;
            break;
        case 1:
        case 3:
        case 7:
        case 9:
        case 12:
        case 14:
            argb = 0xFFE6D03F;
            break;
        case 2:
        case 11:
            argb = 0xFFDC3030;
            break;
        case 4:
            argb = 0xFF50CD20;
            break;
        case 5:
            argb = 0xFF645A14;
            break;
        case 6:
        case 13:
            argb = 0xFF468CDC;
            break;
        case 8:
        case 10:
            argb = 0xFFBEBEBE;
            break;
        case 15:
            argb = 0xFFDC9628;
            break;
        case 16:
            argb = 0xFFD0BF3D;
            break;
        case 17:
            argb = 0xFFC3611C;
            break;
        case 18:
            argb = 0xFF5F73B0;
            break;
    }

    if (unk_2c)
        unk_2c->set_color32(std::bit_cast<color32>(argb));

    is_shown = false;
    unk_3c   = false;

    if (unk_24)
        unk_24->set_shown(false);

    if (unk_2c)
        unk_2c->set_shown(false);

    unk_0c = 1.0f;
    unk_10 = 1.0f;
    state  = 0;
    unk_1c = 0;

    if (this->kind == 5)
        unk_10 = 0.5f;

    unk_30 = unk_2c->get_scale();
    unk_34 = 1.0f;
    unk_38 = 0.25f;
    unk_28 = nullptr;

    if (unk_2c)
        unk_2c->is_life_bar = false;

    if (unk_28)
        unk_28->set_shown(false);
}

// sub_68C540
i32 poi_icon_widget::widget_style(i32 kind) {
    switch (kind) {
        case 0:
        case 16:
            return 0;
        case 1:
        case 14:
        case 18:
            return 1;
        case 17:
            return 2;
        default:
            return 3;
    }
}

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

// sub_68C450
// inlined @ sub_692590
void poi_icon_widget::set_life_bar_values(i32 health, i32 unk) {
    if (unk_2c) {
        unk_2c->life_bar_health = health;
        unk_2c->unk_164         = unk;
    }

    if (unk_28) {
        unk_28->life_bar_health = health;
        unk_28->unk_184         = unk;
    }
}

// sub_68C490
// inlined @ sub_6925D0
i32 poi_icon_widget::get_life_bar_health() {
    if (unk_2c)
        return unk_2c->life_bar_health;

    if (unk_28)
        return unk_28->life_bar_health;

    return -1;
}

// sub_6923F0
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
