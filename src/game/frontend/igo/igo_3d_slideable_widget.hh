#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/types.hh"

namespace treyarch {
    // no vtable of its own and no slots past the widget's
    class igo_3d_slideable_widget : public igo_3d_widget {

    public:
        explicit igo_3d_slideable_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}
    };
} // treyarch
