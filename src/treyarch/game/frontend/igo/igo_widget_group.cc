#include "treyarch/game/frontend/igo/igo_widget_group.hh"

using namespace treyarch;

// sub_68CAA0
void igo_widget_group::draw() {
    for (igo_3d_drawable* widget : widgets) {
        if (widget)
            widget->draw();
    }
}
