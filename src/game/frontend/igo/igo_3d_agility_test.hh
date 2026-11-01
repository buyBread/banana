#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the toa helper's button prompt
    class igo_3d_agility_test : public igo_3d_widget {

    public:
        bool unk_150;
        bool prompt_shown;
        u8   reserved_152[0x0A];
        bool waits_for_press; // the current cut scene's
    };

    ASSERT_OFFSETOF(igo_3d_agility_test, unk_150,         0x150);
    ASSERT_OFFSETOF(igo_3d_agility_test, prompt_shown,    0x151);
    ASSERT_OFFSETOF(igo_3d_agility_test, waits_for_press, 0x15C);
} // treyarch
