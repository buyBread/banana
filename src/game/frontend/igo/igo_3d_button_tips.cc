#include <cstring>

#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_button_tips.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/frontend/ui_frontend.hh"

using namespace treyarch;

// sub_6DF6E0
void button_tip::set(arch_base_vhandle e, const char* button_a, const char* button_b, f32 modifier, f32 unk) {
    ent                        = e;
    unused                     = false;
    y_facing_location_modifier = modifier;
    position.x                 = 0.0f;
    position.y                 = 0.0f;
    position.z                 = 0.0f;
    unk_2c                     = unk;
    unk_30                     = 0;
    unk_34                     = 0;

    igo_3d_face_button_system* buttons = references::frontend.get().igo->face_button_system;

    if (!button_b) {
        if (!buttons)
            return;

        if (button_id_a >= 0 && buttons->has_button(button_id_a))
            buttons->remove_button(button_id_a);

        button_kind_a = retail::sub_6904B0((char*)button_a); // the button kind of a "~name"
        is_web_a      = !_stricmp(button_a, "~web");
    } else {
        if (!buttons)
            return;

        if (button_id_a >= 0 && buttons->has_button(button_id_a))
            buttons->remove_button(button_id_a);

        if (button_id_b >= 0 && buttons->has_button(button_id_b))
            buttons->remove_button(button_id_b);

        button_kind_a = retail::sub_6904B0((char*)button_a); // the button kind of a "~name"
        button_kind_b = retail::sub_6904B0((char*)button_b);
        is_web_a      = !_stricmp(button_a, "~web");
        is_web_b      = !_stricmp(button_b, "~web");
    }
}

// sub_6DF810
void button_tip::remove() {
    unused = true;

    igo_3d_face_button_system* buttons = references::frontend.get().igo->face_button_system;

    if (buttons) {
        if (button_id_a >= 0 && buttons->has_button(button_id_a))
            buttons->remove_button(button_id_a);

        if (button_id_b >= 0 && buttons->has_button(button_id_b))
            buttons->remove_button(button_id_b);
    }

    button_id_a   = -1;
    button_id_b   = -1;
    button_kind_a = -1;
    button_kind_b = -1;
    is_web_a      = false;
    is_web_b      = false;
}

// sub_6DF880
i32 igo_3d_button_tips::add_tip(arch_base_vhandle ent, const char* button_a, const char* button_b, f32 modifier, f32 unk) {
    for (i32 index = 0; index < max_button_tips; ++index) {
        if (tips[index].unused) {
            tips[index].set(ent, button_a, button_b, modifier, unk);

            return index;
        }
    }

    return -1;
}

// sub_6DF8F0
void igo_3d_button_tips::remove_tip(u32 index) {
    if (index <= 19)
        tips[index].remove();
}
