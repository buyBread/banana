#pragma once

#include "vc8/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "game/singleton.hh"
#include "treyarch/mash/string.hh"
#include "game/math/algebra/vector4d.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    // the s1 zombie symbiotes: a few real zombies near the camera and sprites beyond them
    class zombie_manager : public singleton<zombie_manager, 0x0102FFF0> {

    public:
        // a zombie drawn as a sprite; a real one takes its place once it comes close in view
        struct fake_zombie {
            po abs_po;
            u8 alpha;             // 0 for a free record
            i8 fade_step;
            u8 render_flags;      // the sprite faces to draw, picked by the camera's angle
            u8 in_view;
            u8 reserved_44[0x0C];
        };

        vc8::vector<arch_base_vhandle> zombies;              // the real s1_zombie actors, nine of them
        fake_zombie*                   fake_zombies;
        u32                            fake_zombie_count;
        mash::string                   textures[3];          // s1_zombie_anim_side, _front and _top
        u32                            atlas_frame_count[4];
        vector4d                       scale;
        u32                            atlas_frame;
        f32                            atlas_frame_timer;
        u32                            spawns_this_frame;    // fake zombies placed this frame
        u8                             zombies_enabled;      // set by chuck zombie_set_zombies_enabled
        u8                             suspended;            // set by chuck zombie_suspend
        u8                             pad_06e[0x02];

        void frame_advance(f32 time_inc);

        // gives the fake zombie a real one: the first hidden one, else the farthest outside the camera's cone
        void replace_fake_zombie(fake_zombie*    fake,
                                 const vector4d* camera_position,
                                 const vector4d* camera_facing);

        // restarts the zombie where the fake one stands and shows it
        static void place_zombie(actor* zombie, const fake_zombie* fake);

        // the rows tested against the camera direction for the first two render flags
        static constexpr i32 cone_rows[2] = { 2, 0 };
    };

    ASSERT_SIZEOF  (zombie_manager::fake_zombie,               0x50);
    ASSERT_OFFSETOF(zombie_manager::fake_zombie, alpha,        0x40);
    ASSERT_OFFSETOF(zombie_manager::fake_zombie, render_flags, 0x42);
    ASSERT_OFFSETOF(zombie_manager::fake_zombie, in_view,      0x43);

    ASSERT_SIZEOF  (zombie_manager,                    0x70);
    ASSERT_OFFSETOF(zombie_manager, zombies,           0x04);
    ASSERT_OFFSETOF(zombie_manager, fake_zombies,      0x14);
    ASSERT_OFFSETOF(zombie_manager, textures,          0x1C);
    ASSERT_OFFSETOF(zombie_manager, atlas_frame_count, 0x40);
    ASSERT_OFFSETOF(zombie_manager, scale,             0x50);
    ASSERT_OFFSETOF(zombie_manager, atlas_frame,       0x60);
    ASSERT_OFFSETOF(zombie_manager, spawns_this_frame, 0x68);
    ASSERT_OFFSETOF(zombie_manager, zombies_enabled,   0x6C);
    ASSERT_OFFSETOF(zombie_manager, suspended,         0x6D);
} // treyarch
