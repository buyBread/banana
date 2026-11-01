#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the ui_hud_rvb_choice two-choice dialog
    class dialog_box : public igo_generic_input_listener, public igo_3d_widget {

    public:
        bool           shown;
        bool           confirmed;
        u8             choice;         // 0 or 1
        bool           freeze;         // show stores its argument here
        u8             reserved_164[0x04];
        u32            unk_168;        // the front end's update deletes a hidden box only while it's 0
        u8             reserved_16c[0x48];
        igo_3d_widget* updown_button;  // "~updown"
        igo_3d_widget* confirm_button; // "~confirm"
        u8             reserved_1bc[0x04];

        explicit dialog_box(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing),
                                                                             igo_3d_widget(so_do_nothing) {}

        ~dialog_box() override;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void unk_002(i32 unk) override;
        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_005(i32 unk) override;
        void unk_006(i32 unk) override;
        void unk_007(i32 unk) override;

        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;
        i32  unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;

        virtual void show(bool freeze_hero);
        virtual void hide();
    };

    ASSERT_SIZEOF  (dialog_box,                 0x1C0);
    ASSERT_OFFSETOF(dialog_box, shown,          0x160);
    ASSERT_OFFSETOF(dialog_box, confirmed,      0x161);
    ASSERT_OFFSETOF(dialog_box, choice,         0x162);
    ASSERT_OFFSETOF(dialog_box, unk_168,        0x168);
    ASSERT_OFFSETOF(dialog_box, updown_button,  0x1B4);
    ASSERT_OFFSETOF(dialog_box, confirm_button, 0x1B8);
} // treyarch
