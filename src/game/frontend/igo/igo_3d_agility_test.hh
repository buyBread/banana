#pragma once

#include "game/frontend/igo/igo_3d_agility_hotspot.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_face_button_widget;

    // the toa helper's button prompt
    class igo_3d_agility_test : public igo_3d_widget {

    public:
        bool                       x_widget_shown;  // the button's x widget is up
        bool                       prompt_shown;
        u8                         pad_152[0x02];
        f32                        elapsed;
        f32                        window;          // how long the current button can be pressed
        bool                       waits_for_press; // the current cut scene's
        bool                       was_hit;
        u8                         pad_15e[0x02];
        igo_3d_face_button_widget* button;
        igo_3d_agility_hotspot*    hotspot;

        explicit igo_3d_agility_test(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}

        ~igo_3d_agility_test() override;

        // writes the source vptr over a test the retail constructor built, its hotspot too
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_agility_test>();

            hotspot->stamp_source_vtable();
        }

        void init() override;
        void update(f32 time_inc) override;
        void render() override;

        virtual void unk_054();

        void show(bool shown); // showing sets the button 40 above the hotspot, without one above the test itself
        void mark_hit();
    };

    ASSERT_SIZEOF  (igo_3d_agility_test,                  0x170);
    ASSERT_OFFSETOF(igo_3d_agility_test, x_widget_shown,  0x150);
    ASSERT_OFFSETOF(igo_3d_agility_test, prompt_shown,    0x151);
    ASSERT_OFFSETOF(igo_3d_agility_test, elapsed,         0x154);
    ASSERT_OFFSETOF(igo_3d_agility_test, window,          0x158);
    ASSERT_OFFSETOF(igo_3d_agility_test, waits_for_press, 0x15C);
    ASSERT_OFFSETOF(igo_3d_agility_test, was_hit,         0x15D);
    ASSERT_OFFSETOF(igo_3d_agility_test, button,          0x160);
    ASSERT_OFFSETOF(igo_3d_agility_test, hotspot,         0x164);
} // treyarch
