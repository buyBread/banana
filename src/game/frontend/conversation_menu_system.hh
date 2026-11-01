#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class conversation_menu_system : public igo_generic_input_listener, public igo_3d_widget {

    public:
        vc8::vector<u32> unk_160;
        u8               reserved_170[0x04];
        bool             active;
        u8               reserved_175[0xD7];
        u8               unk_24c[8][0x0C];  // eight mash::strings, finalized by the destructor
        u8               reserved_2ac[0x20];
        igo_3d_text*     name_text;         // "ui_HUD_conversation_name_text"
        igo_3d_text*     text;              // "ui_HUD_conversation_text"
        igo_3d_text*     replies_text;      // "ui_HUD_conversation_replies_text"
        igo_3d_widget*   updown_button;     // "~updown"
        igo_3d_widget*   confirm_button;    // "~confirm"
        igo_3d_widget*   cancel_button;     // "~cancel"
        u8               reserved_2e4[0x3C];

        explicit conversation_menu_system(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing),
                                                                                           igo_3d_widget(so_do_nothing) {}

        ~conversation_menu_system() override;

        // writes both source vptrs over a system the retail constructor built
        void stamp_source_vtables() {
            *(mash::vtable_value_t*)this                 = mash::generate_vtable<conversation_menu_system>();
            *(mash::vtable_value_t*)(igo_3d_widget*)this = mash::generate_vtable<conversation_menu_system, igo_3d_widget>();
        }

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
        void unk_010(i32 unk) override;

        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;
        i32  unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;
        i32  unk_047(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) override;

        bool is_conversation_active();
    };

    ASSERT_SIZEOF  (conversation_menu_system,                0x320);
    ASSERT_OFFSETOF(conversation_menu_system, unk_160,       0x160);
    ASSERT_OFFSETOF(conversation_menu_system, active,        0x174);
    ASSERT_OFFSETOF(conversation_menu_system, unk_24c,       0x24C);
    ASSERT_OFFSETOF(conversation_menu_system, name_text,     0x2CC);
    ASSERT_OFFSETOF(conversation_menu_system, cancel_button, 0x2E0);
} // treyarch
