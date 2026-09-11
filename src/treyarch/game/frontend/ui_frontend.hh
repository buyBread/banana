#pragma once

#include "treyarch/game/frontend/igo/igo_3d_zoom_map.hh"
#include "treyarch/shared/container/legacy_list.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class ui_frontend;

    using ui_frontend_method = void (__thiscall*)(ui_frontend* self);

    struct ui_frontend_vtable {
        void*              reserved_000[9];
        ui_frontend_method draw;
        ui_frontend_method draw_startup;
        ui_frontend_method draw_quad_list;
    };

    class ui_frontend {

    public:
        ui_frontend_vtable*           vtable;
        u8                            reserved_004[0x128];
        igo_3d_zoom_map*              zoom_map;
        u8                            reserved_130[0xD0];
        u8                            quad_list_state_200;
        u8                            reserved_201[0x03];
        container::legacy_list<void*> quad_list;

        void draw();
        void draw_startup();
        void draw_quad_list();
        void clear_quad_list();

        bool blocks_world_rendering() const {
            return zoom_map->blocks_world_rendering();
        }
    };

    ASSERT_SIZEOF  (ui_frontend_vtable,                 0x30);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw,           0x24);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_startup,   0x28);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_quad_list, 0x2C);

    ASSERT_OFFSETOF(ui_frontend, zoom_map,            0x12C);
    ASSERT_OFFSETOF(ui_frontend, quad_list_state_200, 0x200);
    ASSERT_OFFSETOF(ui_frontend, quad_list,           0x204);
} // treyarch
