#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class combo_interface : public actor_interface {

    public:
        virtual void frame_advance(f32 delta_t); // decays the combo meter
    };
} // treyarch
