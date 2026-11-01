#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_script_widget : public igo_3d_widget {

    public:
        f32                     continuous_rotation;
        f32                     continuous_tilt;
        f32                     continuous_spin;
        vc8::list<igo_3d_text*> texts;
        u8                      unk_168;
        u8                      pad_169[0x03];
        f32                     max_timer;
        u8                      reserved_170[0x08];
        string_hash             text_parameters_key; // every text is built with it
        u8                      unk_17c;
        bool                    persistent;          // drawn while the interface is disabled
        u8                      pad_17e[0x02];
        u32                     instance_id;
        u8                      reserved_184[0x0C];

        virtual void  unk_054(f32 unk) = 0;
        virtual void  set_continuous_rotation(f32 rate) = 0;
        virtual void  set_continuous_tilt(f32 rate) = 0;
        virtual void  set_continuous_spin(f32 rate) = 0;
        virtual void* unk_058(void* unk_0, void* unk_1) = 0;
        virtual void  unk_059(i32 unk) = 0;
        virtual u8    unk_060() = 0; // returns unk_168
        virtual u32   get_instance_id() = 0;

        igo_3d_text* get_text(i32 id); // made on first use; -2 answers null
    };

    ASSERT_SIZEOF  (igo_3d_script_widget,                      0x190);
    ASSERT_OFFSETOF(igo_3d_script_widget, continuous_rotation, 0x150);
    ASSERT_OFFSETOF(igo_3d_script_widget, texts,               0x15C);
    ASSERT_OFFSETOF(igo_3d_script_widget, unk_168,             0x168);
    ASSERT_OFFSETOF(igo_3d_script_widget, max_timer,           0x16C);
    ASSERT_OFFSETOF(igo_3d_script_widget, text_parameters_key, 0x178);
    ASSERT_OFFSETOF(igo_3d_script_widget, persistent,          0x17D);
    ASSERT_OFFSETOF(igo_3d_script_widget, instance_id,         0x180);
} // treyarch
