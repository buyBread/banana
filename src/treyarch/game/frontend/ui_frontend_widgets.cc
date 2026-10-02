#include <new>

#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_scrapbook.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/shared/mash/string.hh"

using namespace treyarch;

// sub_6CD220
void ui_frontend::draw_widgets() {
    for (auto* position = widgets.begin(); position != widgets.end(); position = position->next) {
        igo_3d_widget* widget = position->value;

        if (!widget || !widget->is_visible())
            continue;

        u32 mask = widget->suppression_mask();

        if (!references::game.read()->disable_interface && !(mask & widget_suppression_conditions))
            widget->draw();
    }
}

// sub_6F1FD0
void ui_frontend::draw_script_widgets() {
    if (select_ui_environment_index())
        return;

    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = script_widgets.begin(); position != script_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;
        input_mgr*            inputs = references::input_manager.read();

        input_device* device = inputs->devices[joystick_2_device];

        if (!device)
            device = inputs->devices[joystick_1_device];

        if (device && device->unk_034()) {
            if (!widget)
                continue;

            // children labelled "[...]" get method_008(1);
            // a widget without children is hidden
            dinkumware::list<igo_3d_script_widget_child*> children(widget->children);

            if (children.size()) {
                for (auto* child = children.begin(); child != children.end(); child = child->next) {
                    igo_3d_script_widget_child* value = child->value;

                    bool bracketed = false;

                    if (value) {
                        alignas(mash::string) u8 storage[sizeof(mash::string)];

                        mash::string* label = value->vtable->get_label(value, (mash::string*)storage);

                        bracketed = label->data()[0] == '[';

                        // the temporary dies through this twin of the string destructor, not sub_A6CEE0
                        retail::sub_A6CCA0(label, 0);
                    }

                    if (bracketed)
                        value->vtable->method_008(value, 1);
                }
            } else
                widget->set_visible(false);
        }

        if (widget && widget->is_visible() && !widget->unk_0f0())
            widget->draw();
    }

    draw_mission_text_widget();
    draw_hint_text_widget();
}

// sub_6CCFC0
void ui_frontend::draw_mission_text_widget() {
    ui_frontend* current_igo = references::frontend.get().igo;

    if (!current_igo->scrapbook->is_active() && current_igo->conversation_menu_system->active)
        return;

    if (current_igo->pauseless_dialog->unk_0d8)
        return;

    for (auto* position = mission_text_widgets.begin(); position != mission_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;

        if (widget && widget->is_visible() && !widget->unk_0f0()) {
            widget->draw();

            return;
        }
    }
}

// sub_6CD050
void ui_frontend::draw_hint_text_widget() {
    ui_frontend* current_igo = references::frontend.get().igo;

    if (!current_igo->scrapbook->is_active() && current_igo->conversation_menu_system->active)
        return;

    if (current_igo->pauseless_dialog->unk_0d8)
        return;

    for (auto* position = hint_text_widgets.begin(); position != hint_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;

        if (widget && widget->is_visible() && !widget->unk_0f0()) {
            widget->draw();

            return;
        }
    }
}

// sub_6CD0E0
void ui_frontend::draw_interface_disabled_script_widgets(bool interface_disabled) {
    if (!interface_disabled || select_ui_environment_index())
        return;

    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = script_widgets.begin(); position != script_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;

        if (widget && widget->is_visible() && !widget->unk_0f0() && widget->unk_17d)
            widget->draw();
    }
}
