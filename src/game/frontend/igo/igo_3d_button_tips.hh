#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // one of the button tips' records
    struct button_tip {
        i32               button_id_a;   // face buttons of the face button system
        i32               button_id_b;
        i32               button_kind_a;
        i32               button_kind_b;
        bool              is_web_a;
        bool              is_web_b;
        u8                pad_12[0x02];
        arch_base_vhandle ent;
        bool              unused;
        u8                pad_19[0x03];
        f32               y_facing_location_modifier;
        vector3d          position;
        f32               unk_2c;
        u32               unk_30;
        u32               unk_34;

        void set(arch_base_vhandle e, const char* button_a, const char* button_b, f32 modifier, f32 unk); // button_b may be null
        void remove(); // also takes its face buttons away
    };

    class igo_3d_button_tips : public igo_3d_widget {

    public:
        static constexpr i32 max_button_tips = 20;

        button_tip tips[max_button_tips];

        i32  add_tip(arch_base_vhandle ent, const char* button_a, const char* button_b, f32 modifier, f32 unk); // -1 when all are in use
        void remove_tip(u32 index);
    };

    ASSERT_SIZEOF  (button_tip,                             0x38);
    ASSERT_OFFSETOF(button_tip, is_web_a,                   0x10);
    ASSERT_OFFSETOF(button_tip, ent,                        0x14);
    ASSERT_OFFSETOF(button_tip, unused,                     0x18);
    ASSERT_OFFSETOF(button_tip, y_facing_location_modifier, 0x1C);
    ASSERT_OFFSETOF(button_tip, unk_2c,                     0x2C);

    ASSERT_SIZEOF  (igo_3d_button_tips,       0x5B0);
    ASSERT_OFFSETOF(igo_3d_button_tips, tips, 0x150);
} // treyarch
