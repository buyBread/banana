#include <bit>

#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/igo_map_poi.hh"

using namespace treyarch;

// sub_6FFAA0
igo_map_poi::igo_map_poi(i32 kind, u32 unk_14, vector3d) { // the position goes unused
    is_shown = false;

    for (u8 &unk : unk_19)
        unk = 0;

    this->unk_14 = unk_14;
    this->kind   = kind;
    position     = vector3d(0.0f, 0.0f, 0.0f);
    unk_48       = 0;

    u32 argb = 0xFFB4B4B4;

    switch (kind) {
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
            argb = 0xFFA03030;
            break;
        case 4:
        case 10:
            argb = 0xFF50CD20;
            break;
        case 5:
            argb = 0xFF645A14;
            break;
        case 6:
        case 11:
        case 13:
            argb = 0xFF468CDC;
            break;
        case 8:
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

    set_color(std::bit_cast<color32>(argb));

    igo_3d_widget* radar = references::frontend.get().igo->radar_widget;

    if (kind == 15)
        retail::sub_6FC010(radar, (i32)this);
    else
        retail::sub_6FBF90((u32*)radar, (i32)this);

    is_shown  = true;
    unk_19[0] = 1;
    unk_19[1] = 1;
    unk_19[2] = 0;
    unk_19[3] = 0;
    unk_19[4] = 1;
    unk_20    = 0.0f;
    unk_24    = 0.0f;
    unk_30    = 0.0f;
    unk_34    = 0.0f;
    unk_38    = 0.0f;

    for (u8 &unk : unk_28)
        unk = 0;

    unk_3c = false;
    unk_44 = 0;
    unk_4c = 0;
    unk_40 = 1.0f;
}

// sub_6A9C40
void igo_map_poi::set_color(color32 new_color) {
    color.set_clamped(new_color.r, new_color.g, new_color.b, new_color.a);
}
