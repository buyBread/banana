#include "retail.hh"
#include "game/frontend/igo/igo_3d_agility_test.hh"
#include "game/frontend/igo/igo_3d_face_button_widget.hh"

using namespace treyarch;

// sub_6FC1A0
igo_3d_agility_test::~igo_3d_agility_test() {
    if (button) {
        delete button;
        button = nullptr;
    }

    if (hotspot) {
        delete hotspot;
        hotspot = nullptr;
    }
}

// slot 1
void igo_3d_agility_test::init() {
    ((void (__thiscall*)(igo_3d_agility_test*))retail::sub_6921A0)(this);
}

// slot 2
void igo_3d_agility_test::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_agility_test*, f32))retail::sub_6AA600)(this, time_inc);
}

// sub_6922F0
void igo_3d_agility_test::render() {
    if (!is_shown())
        return;

    if (button)
        button->render();

    if (hotspot)
        hotspot->render();
}

// slot 54
void igo_3d_agility_test::unk_054() {}

// sub_6AAA00
void igo_3d_agility_test::show(bool shown) {
    if (prompt_shown) {
        if (!shown) {
            button->set_shown(false);

            if (x_widget_shown && button)
                button->destroy_x_widget();

            x_widget_shown = false;
        }
    } else if (shown) {
        button->set_shown(true);

        elapsed = 0.0f;

        igo_3d_widget* anchor = hotspot ? (igo_3d_widget*)hotspot : this;

        f32 y = anchor->get_position_y();
        f32 x = anchor->get_position_x();

        button->set_center_pos((f32)((f64)x - 0.0), (f32)((f64)y - 40.0));

        if (x_widget_shown && button)
            button->destroy_x_widget();

        x_widget_shown = false;

        if (hotspot) {
            hotspot->hit         = false;
            hotspot->hit_elapsed = 0.0f;
        }

        was_hit = false;
    }

    prompt_shown = shown;

    set_shown(shown);
}

// sub_692330
void igo_3d_agility_test::mark_hit() {
    if (hotspot && !hotspot->hit) {
        hotspot->hit         = true;
        hotspot->hit_elapsed = 0.0f;
    }

    was_hit = true;
}
