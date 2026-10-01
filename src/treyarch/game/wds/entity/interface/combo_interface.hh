#pragma once

#include "treyarch/game/wds/entity/interface/generic_interface.hh"
#include "util/types.hh"

namespace treyarch {
    class combo_interface;

    struct combo_interface_vtable : generic_interface_vtable {
        void (__thiscall* frame_advance)(combo_interface* self, f32 delta_t); // sub_67ACC0; decays the combo meter
    };

    class combo_interface : public actor_interface {

    public:
        combo_interface_vtable* get_vtable() const {
            return (combo_interface_vtable*)vtable;
        }

        void frame_advance(f32 delta_t) {
            get_vtable()->frame_advance(this, delta_t);
        }
    };
} // treyarch
