#include "retail.hh"
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

    retail::sub_6D8270((u32*)this, (i32*)widget); // ui_frontend::destroy_script_widget

    return false;
}
