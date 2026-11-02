#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // what the agility test's button sits over, "ui_hud_toa_drop_hit"
    class igo_3d_agility_hotspot : public igo_3d_widget {

    public:
        i32  rotator_member;  // "ui_TOA_drop_rotator"
        i32  glow_1_member;   // "ui_TOA_drop_glow_1"
        i32  glow_2_member;   // "ui_TOA_drop_glow_2"
        i32  glow_web_member; // "ui_TOA_drop_glow_web"
        f32  hit_elapsed;     // runs up to 0.2 once hit, 0 before that
        bool hit;
        u8   pad_165[0x03];
        u32  unk_168;

        explicit igo_3d_agility_hotspot(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}

        // writes the source vptr over a hotspot the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_agility_hotspot>();
        }

        void update(f32 time_inc) override;
        void spawn_entity() override;

        void render_congomerate_mesh(ngl::mesh*              mesh,
                                     i32                     index,
                                     const matrix4x4        &trans_matrix,
                                     const matrix4x4        &rel_pos_matrix,
                                     ngl::mesh_params*       mesh_params,
                                     ngl::shader_param_set*  shader_params) override;
    };

    ASSERT_SIZEOF  (igo_3d_agility_hotspot,                  0x170);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, rotator_member,  0x150);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, glow_1_member,   0x154);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, glow_2_member,   0x158);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, glow_web_member, 0x15C);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, hit_elapsed,     0x160);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, hit,             0x164);
    ASSERT_OFFSETOF(igo_3d_agility_hotspot, unk_168,         0x168);
} // treyarch
