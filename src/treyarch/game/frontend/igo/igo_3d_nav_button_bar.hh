#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"

namespace treyarch {
    class igo_3d_nav_button_bar : public igo_3d_drawable {
        
    public:
        void draw_buttons() {
            vtable->method_014(this);
        }
    };
} // treyarch
