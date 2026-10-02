#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"

namespace treyarch {
    class igo_3d_face_button_system : public igo_3d_drawable {
        
    public:
        void draw_buttons() {
            vtable->method_06c(this);
        }
    };
} // treyarch
