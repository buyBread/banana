#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class combo_interface : public actor_interface {

    public:
        u8  reserved_00c[0x28];
        u32 unk_034;            // carried in a save
        u32 unk_038;            // carried in a save
        f32 combo_meter;

        virtual void frame_advance(f32 delta_t); // decays the combo meter
    };

    ASSERT_OFFSETOF(combo_interface, unk_034,     0x34);
    ASSERT_OFFSETOF(combo_interface, combo_meter, 0x3C);
} // treyarch
