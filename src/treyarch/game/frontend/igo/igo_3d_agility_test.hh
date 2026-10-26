#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the toa helper's button prompt
    class igo_3d_agility_test : public igo_3d_widget {

    public:
        u8   reserved_150[0x0C];
        bool unk_15c;
    };

    ASSERT_OFFSETOF(igo_3d_agility_test, unk_15c, 0x15C);
} // treyarch
