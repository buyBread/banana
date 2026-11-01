#include "game/frontend/ui_frontend.hh"

using namespace treyarch;

// sub_6D85F0
bool ui_frontend::is_script_widget_valid(igo_3d_script_widget* widget) {
    if (!widget)
        return false;

    igo_3d_script_widget* listed;

    {
        engine_lock_scope scope(&references::script_widget_lock.get());

        listed = find_script_widget(script_widgets, widget);

        if (!listed)
            listed = find_script_widget(mission_text_widgets, widget);

        if (!listed)
            listed = find_script_widget(hint_text_widgets, widget);

        // both instance ids are read, but nothing compares them
        if (listed) {
            listed->get_instance_id();
            widget->get_instance_id();
        }
    }

    if (!listed)
        return false;

    if (widget->entity_vh.resolve())
        return true;

    destroy_script_widget(widget);

    return false;
}

// sub_6D8270
void ui_frontend::destroy_script_widget(igo_3d_script_widget* widget) {
    engine_lock_scope scope(&references::script_widget_lock.get());

    if (!widget)
        return;

    for (auto* position = script_widgets.begin(); position != script_widgets.end(); position = position->next) {
        igo_3d_script_widget* listed = position->value;

        if (listed != widget)
            continue;

        script_widgets.erase(position);
        widget->reset_anim_speeds();

        u32 listed_id = listed->get_instance_id();

        if (widget->get_instance_id() == listed_id)
            delete widget;

        return;
    }

    if (!destroy_mission_text_widget(widget))
        destroy_hint_text_widget(widget);
}

// sub_6D7E70
bool ui_frontend::destroy_mission_text_widget(igo_3d_script_widget* widget) {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = mission_text_widgets.begin(); position != mission_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* listed = position->value;

        if (listed != widget)
            continue;

        mission_text_widgets.erase(position);
        widget->reset_anim_speeds();

        u32 listed_id = listed->get_instance_id();

        if (widget->get_instance_id() != listed_id)
            return false;

        delete widget;

        return true;
    }

    return false;
}

// sub_6D8070
bool ui_frontend::destroy_hint_text_widget(igo_3d_script_widget* widget) {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = hint_text_widgets.begin(); position != hint_text_widgets.end(); position = position->next) {
        igo_3d_script_widget* listed = position->value;

        if (listed != widget)
            continue;

        hint_text_widgets.erase(position);
        widget->reset_anim_speeds();

        u32 listed_id = listed->get_instance_id();

        if (widget->get_instance_id() != listed_id)
            return false;

        delete widget;

        return true;
    }

    return false;
}

// sub_6D8380
void ui_frontend::destroy_all_script_widgets() {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = script_widgets.begin(); position != script_widgets.end();) {
        igo_3d_script_widget* widget = position->value;

        position = script_widgets.erase(position);

        if (widget) {
            widget->reset_anim_speeds();
            delete widget;
        }
    }

    destroy_all_mission_text_widgets();
    destroy_all_hint_text_widgets();
}

// sub_6D7F90
void ui_frontend::destroy_all_mission_text_widgets() {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = mission_text_widgets.begin(); position != mission_text_widgets.end();) {
        igo_3d_script_widget* widget = position->value;

        position = mission_text_widgets.erase(position);

        if (widget) {
            widget->reset_anim_speeds();
            delete widget;
        }
    }
}

// sub_6D8190
void ui_frontend::destroy_all_hint_text_widgets() {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = hint_text_widgets.begin(); position != hint_text_widgets.end();) {
        igo_3d_script_widget* widget = position->value;

        position = hint_text_widgets.erase(position);

        if (widget) {
            widget->reset_anim_speeds();
            delete widget;
        }
    }
}

// sub_6CCEA0
void ui_frontend::hide_all_mission_text_widgets() {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = mission_text_widgets.begin(); position != mission_text_widgets.end(); position = position->next)
        if (position->value)
            position->value->set_shown(false);
}

// sub_6CCF30
void ui_frontend::hide_all_hint_text_widgets() {
    engine_lock_scope scope(&references::script_widget_lock.get());

    for (auto* position = hint_text_widgets.begin(); position != hint_text_widgets.end(); position = position->next)
        if (position->value)
            position->value->set_shown(false);
}
