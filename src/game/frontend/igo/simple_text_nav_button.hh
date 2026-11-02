#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;

    class simple_nav_button {

    public:
        i32  member_a;
        i32  member_b;
        bool selected;
        u8   pad_0d[0x03];
        f32  selected_time;
        f32  selected_time_limit; // the update deselects at it

        explicit simple_nav_button(mash::generating_vtable_version) {}

        virtual ~simple_nav_button() = default;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void nav_select(f32 unk);
        virtual void update(f32 time_inc);
    };

    // a nav button with an igo_3d_text label
    class simple_text_nav_button : public simple_nav_button {

    public:
        igo_3d_text* text;             // "ui_shell_dialog_button_text"
        f32          unk_1c;
        i32          unk_20;
        u8           reserved_24[0x04];
        bool         shown;
        u8           pad_29[0x03];

        explicit simple_text_nav_button(mash::generating_vtable_version so_do_nothing) : simple_nav_button(so_do_nothing) {}

        ~simple_text_nav_button() override;

        // writes the source vptr over a button the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<simple_text_nav_button>();
        }

        void nav_select(f32 unk) override;
        void update(f32 time_inc) override;

        virtual void nav_deselect(f32 unk);
        virtual void set_shown(bool shown); // shows or hides the label

        void set_text(mash::string* value); // destroys the value
    };

    ASSERT_SIZEOF  (simple_nav_button,                      0x18);
    ASSERT_OFFSETOF(simple_nav_button, member_a,            0x04);
    ASSERT_OFFSETOF(simple_nav_button, member_b,            0x08);
    ASSERT_OFFSETOF(simple_nav_button, selected,            0x0C);
    ASSERT_OFFSETOF(simple_nav_button, selected_time,       0x10);
    ASSERT_OFFSETOF(simple_nav_button, selected_time_limit, 0x14);

    ASSERT_SIZEOF  (simple_text_nav_button,         0x2C);
    ASSERT_OFFSETOF(simple_text_nav_button, text,   0x18);
    ASSERT_OFFSETOF(simple_text_nav_button, unk_1c, 0x1C);
    ASSERT_OFFSETOF(simple_text_nav_button, unk_20, 0x20);
    ASSERT_OFFSETOF(simple_text_nav_button, shown,  0x28);
} // treyarch
