#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/dinkumware/map.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class ui_object_display_1c {

    public:
        u8               reserved_000[0x08];
        u32              state;          // the +0x24 drawable shows in states 1 and 2
        u8               reserved_00c[0x11];
        bool             enabled;
        u8               reserved_01e[0x06];
        igo_3d_drawable* unk_24;
        igo_3d_drawable* unk_28;
        igo_3d_drawable* unk_2c;
        u8               reserved_030[0x0C];
        bool             unk_3c;

        void draw();
    };

    class ui_object_display_30 {

    public:
        u32              unk_00;
        u32              state;          // the +0x0C drawable shows in states 1 and 2
        igo_3d_drawable* unk_08;
        igo_3d_drawable* unk_0c;
        u8               reserved_010[0x0E];
        bool             enabled;

        void draw();
    };

    class ui_object {

    public:
        u32                   unk_00;
        arch_base_vhandle     owner;
        u8                    reserved_008[0x14];
        ui_object_display_1c* display_1c;
        u8                    reserved_020[0x10];
        ui_object_display_30* display_30;
        bool                  shown_in_mode_4;  // read when UIFrontEnd's background-effect mode is 4
        bool                  shown;            // read in every other mode
        u8                    reserved_036[0x17];
        bool                  unk_4d;
    };

    // one direct-mapped slot per masked key
    struct ui_object_table {
        struct entry {
            u32        key;
            ui_object* object;
        };

        u32                   mask;
        u32                   unk_04;
        u32                   capacity;
        entry*                entries;
        u32                   unk_10;
        u32                   requested_capacity;
        u8                    reserved_018[0x28];
        engine_recursive_lock lock;
    };

    class ui_object_manager {

    public:
        dinkumware::map<u32, u32> keys;            // values index `objects`
        u32                       unk_0c;
        ui_object_table           objects;
        u32                       unk_60;
        i32                       unk_64;
        u8                        unk_68[0x0C];    // another VC8 tree, head at +0x6C
        u32                       unk_74;
        ngl::quad*                quad;
        bool                      unk_7c;          // when set, only objects with ui_object::unk_4d use display_1c

        void draw();
    };

    ASSERT_OFFSETOF(ui_object_display_1c, state,   0x08);
    ASSERT_OFFSETOF(ui_object_display_1c, enabled, 0x1D);
    ASSERT_OFFSETOF(ui_object_display_1c, unk_24,  0x24);
    ASSERT_OFFSETOF(ui_object_display_1c, unk_3c,  0x3C);

    ASSERT_OFFSETOF(ui_object_display_30, unk_08,  0x08);
    ASSERT_OFFSETOF(ui_object_display_30, enabled, 0x1E);

    ASSERT_OFFSETOF(ui_object, owner,           0x04);
    ASSERT_OFFSETOF(ui_object, display_1c,      0x1C);
    ASSERT_OFFSETOF(ui_object, display_30,      0x30);
    ASSERT_OFFSETOF(ui_object, shown_in_mode_4, 0x34);
    ASSERT_OFFSETOF(ui_object, unk_4d,          0x4D);

    ASSERT_SIZEOF  (ui_object_table,          0x50);
    ASSERT_OFFSETOF(ui_object_table, entries, 0x0C);
    ASSERT_OFFSETOF(ui_object_table, lock,    0x40);

    ASSERT_SIZEOF  (ui_object_manager,          0x80);
    ASSERT_OFFSETOF(ui_object_manager, objects, 0x10);
    ASSERT_OFFSETOF(ui_object_manager, quad,    0x78);
    ASSERT_OFFSETOF(ui_object_manager, unk_7c,  0x7C);
} // treyarch
