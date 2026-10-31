#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_3d_enemy_health_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_enemy_health_manager {

    public:
        void*                       vtable;
        igo_3d_enemy_health_widget* boss_widget;
        igo_3d_enemy_health_widget* grunt_widget;
        bool                        boss_enabled;
        bool                        boss_icon_set;
        u8                          pad_0e[0x02];
        arch_base_vhandle           boss_entity;
        u8                          reserved_14[0x04];
        bool                        grunt_enabled;
        u8                          pad_19[0x03];

        void draw();

        void set_boss_enabled(bool enabled); // disabling also clears boss_icon_set
    };

    ASSERT_SIZEOF  (igo_3d_enemy_health_manager,                0x1C);
    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, grunt_widget,  0x08);
    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, boss_enabled,  0x0C);
    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, boss_icon_set, 0x0D);
    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, boss_entity,   0x10);
    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, grunt_enabled, 0x18);
} // treyarch
