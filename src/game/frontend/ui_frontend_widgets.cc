#include <new>

#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/input/input_mgr.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_6CD220
void ui_frontend::draw_widgets() {
    for (auto* position = widgets.begin(); position != widgets.end(); position = position->next) {
        igo_3d_widget* widget = position->value;

        if (!widget || !widget->is_shown())
            continue;

        u32 mask = widget->suppression_mask();

        if (!references::game.read()->disable_interface && !(mask & widget_suppression_conditions))
            widget->render();
    }
}

// sub_6F1FD0
void ui_frontend::draw_script_widgets() {
    if (select_ui_environment_index())
        return;

    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = script_widgets.begin(); position != script_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;
        input_mgr*            inputs = input_mgr::inst();

        input_device* device = inputs->devices[joystick_2_device];

        if (!device)
            device = inputs->devices[joystick_1_device];

        if (device && device->unk_034()) {
            if (!widget)
                continue;

            // texts reading "[...]" are shown;
            // a widget without texts is hidden
            vc8::list<igo_3d_text*> texts(widget->texts);

            if (texts.size()) {
                for (auto* text_position = texts.begin(); text_position != texts.end(); text_position = text_position->next) {
                    igo_3d_text* text = text_position->value;

                    bool bracketed = false;

                    if (text) {
                        alignas(mash::string) u8 storage[sizeof(mash::string)];

                        mash::string* label = text->get_text((mash::string*)storage);

                        bracketed = label->data()[0] == '[';

                        // the temporary dies through finalize, the destructor's twin
                        label->finalize(mash::ALLOCATED);
                    }

                    if (bracketed)
                        text->set_shown(true);
                }
            } else
                widget->set_shown(false);
        }

        if (widget && widget->is_shown() && !widget->unk_0f0())
            widget->render();
    }

    draw_mission_text_widget();
    draw_hint_text_widget();
}

// sub_6CCFC0
void ui_frontend::draw_mission_text_widget() {
    ui_frontend* current_igo = references::frontend.get().igo;

    if (!current_igo->scrapbook->is_active() && current_igo->conversation_menu_system->active)
        return;

    if (current_igo->pauseless_dialog->faded_in)
        return;

    for (auto* position = mission_text_widgets.begin(); position != mission_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;

        if (widget && widget->is_shown() && !widget->unk_0f0()) {
            widget->render();

            return;
        }
    }
}

// sub_6CD050
void ui_frontend::draw_hint_text_widget() {
    ui_frontend* current_igo = references::frontend.get().igo;

    if (!current_igo->scrapbook->is_active() && current_igo->conversation_menu_system->active)
        return;

    if (current_igo->pauseless_dialog->faded_in)
        return;

    for (auto* position = hint_text_widgets.begin(); position != hint_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* widget = position->value;

        if (widget && widget->is_shown() && !widget->unk_0f0()) {
            widget->render();

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

        if (widget && widget->is_shown() && !widget->unk_0f0() && widget->persistent)
            widget->render();
    }
}
