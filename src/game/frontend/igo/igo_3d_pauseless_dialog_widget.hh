#pragma once

#include "game/frontend/igo/igo_3d_dialog_widget.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_pauseless_dialog_widget : public igo_3d_dialog_widget {

    public:
        f32 shown_time;       // the update adds to it while the dialog is shown
        u8  reserved_234[0x0C];
        f32 unk_240;
        u8  reserved_244[0x04];
        u8  unk_248;          // set when the update raises its event, as unk_240 reaches 1.5
        u8  unk_249;
        u8  pad_24a[0x02];
        i32 unk_24c;

        explicit igo_3d_pauseless_dialog_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_dialog_widget(so_do_nothing) {}

        // writes both source vptrs over a dialog the retail constructor built, its two buttons too
        void stamp_source_vtables() {
            *(mash::vtable_value_t*)this                              = mash::generate_vtable<igo_3d_pauseless_dialog_widget>();
            *(mash::vtable_value_t*)(igo_generic_input_listener*)this = mash::generate_vtable<igo_3d_pauseless_dialog_widget, igo_generic_input_listener>();

            button_1->stamp_source_vtable();
            button_2->stamp_source_vtable();
        }

        void update(f32 time_inc) override;

        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_007(i32 unk) override;
        void unk_010(i32 unk) override;

        void clear() override;
        void close() override;
    };

    ASSERT_SIZEOF  (igo_3d_pauseless_dialog_widget,             0x250);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, shown_time, 0x230);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_240,    0x240);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_248,    0x248);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_249,    0x249);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_24c,    0x24C);
} // treyarch
