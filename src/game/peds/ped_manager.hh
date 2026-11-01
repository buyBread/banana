#pragma once

#include <array>

#include "vc8/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/math/algebra/vector4d.hh"
#include "game/math/po.hh"
#include "game/mutex.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class entity;

    // where a ped walks; 0xFFFF in any of the first three is no path
    struct ped_path {
        u16      unk_00;
        u16      unk_02;
        u16      road;         // the gb_map road it runs along
        u8       unk_06;
        u8       pad_07;
        u16      flags;        // bit 1 keeps a ped on it fully running
        u8       pad_0a[0x02];
        vector3d unk_0c[2];
        f32      unk_24;
    };

    // a distant ped drawn as a sprite; the fake ped job hands it to a hidden real ped once it comes close
    struct fake_ped {
        po        abs_po;
        vector3d  destination;
        ped_path* path;
        u8        alpha;             // 0 for a free record
        u8        unk_51;
        u8        unk_52;            // alpha as of the last fade step
        i8        fade_step;
        u8        render_flags;      // the sprite faces to draw, picked by the camera's angle
        u8        unk_55;            // cleared when the record is placed
        u8        unk_56;            // 0 and 1 take a real ped whose name has no 'f' at index 6, 2 one that has
        u8        in_view;
        u8        reserved_58[0x08];

        // inlined @ sub_757C70
        void step_fade() {
            i32 stepped = fade_step + alpha;

            if (stepped <= 0) {
                alpha     = 0;
                fade_step = 0;
            } else if (stepped > 0xFF) {
                alpha     = 0xFF;
                fade_step = 0;
            } else
                alpha = (u8)stepped;

            unk_52 = alpha;
        }
    };

    // one of the spawn quad search's two result buffers
    struct ped_spawn_quad_search_buffer {
        u8  reserved_00[0x28];
        i32 unk_28;
        u8  reserved_2c[0x04];
    };

    class ped_manager {

    public:
        static void frame_advance(f32 time_inc); // the pedestrians step of the world frame

        // ped_jq's work: spawns a hidden ped's placement, or culls, unspawns and sets up a visible one by distance
        static bool advance_ped(actor*          ped,
                                const vector4d* the_focus,
                                const vector4d* the_camera_position,
                                const vector4d* the_camera_facing);

        // vendor_jq's work: wakes a hidden vendor in range, or sets up a visible one by distance and puts it to sleep out of range
        static bool advance_vendor(actor*          vendor,
                                   const vector4d* the_focus,
                                   const vector4d* the_camera_position,
                                   const vector4d* the_camera_facing);

        static void wake_vendor (entity* vendor);
        static void sleep_vendor(entity* vendor);

        static void unspawn_ped(actor* ped); // hides it, resets its ai and suspends it

        // without find_spot the ped is shown where it stands; with it, a spot ahead of the camera is searched and the ped placed there
        static bool spawn_ped(actor*          ped,
                              const vector4d* the_camera_position,
                              const vector4d* the_camera_facing,
                              bool            find_spot,
                              bool            check_distance);

        // restarts the ped's ai at a position, facing away from facing, and sends it along its path or to destination
        static bool place_ped(actor*          ped,
                              const vector4d* the_camera_position,
                              const vector4d* the_camera_facing,
                              bool            on_quad_path,
                              i32             quad_path,
                              i32             quad_path_unk,
                              const ped_path* path,
                              f32             path_offset,
                              const vector3d* position,
                              vector3d*       facing,
                              const vector3d* destination);

        // fake_ped_jq's work: a hidden real ped takes the fake ped's place
        static void replace_fake_ped(fake_ped*       fake,
                                     const vector4d* the_camera_position,
                                     const vector4d* the_camera_facing);

        // a negative radius takes any distance and a negative max_count every ped; the handles go to found
        static i32 find_peds_near(const vector3d* center,
                                  f32             radius,
                                  i32             max_count,
                                  i32             flags,
                                  u32*            found);

        static void add_ped          (arch_base_vhandle* ped, bool at_front);
        static void remove_ped_entity(arch_base_vhandle* ped);
        static void clear_in_radius  (const vector3d* center, f32 radius);

        // the frame's view of the world, w 0: the hero's position (the camera's without one), the camera's facing and position
        alignas(16) static inline vector4d focus;
        alignas(16) static inline vector4d camera_facing;
        alignas(16) static inline vector4d camera_position;

        // per-frame ped counts with unk_0102fee0; the ped jobs bump them across threads
        static inline i32 unk_0102fedc; // last frame's unk_0102fee0
        static inline i32 unk_0102fee4; // peds in cars
        static inline i32 unk_0102fee8;
        static inline i32 unk_0102feec; // spawns
        static inline u32 unk_01036e4c; // the world frame the counts belong to

        static inline u32 unk_01030090;
        static inline u32 unk_01030094;

        // fake peds replaced this frame, at most 40, and the world frame that count belongs to
        static inline i32 unk_01032084;
        static inline u32 unk_00e7adcc = 0xFFFFFFFF;
    };

    namespace references {
        inline util::memory_reference<vc8::vector<arch_base_vhandle>> ped_entities    { 0x01030B10 };
        inline util::memory_reference<vc8::vector<arch_base_vhandle>> vendor_entities { 0x01031DD0 };

        inline util::memory_reference<bool> peds_enabled { 0x0102FED8 };

        // the chuck peds_set_* natives write them
        inline util::memory_reference<i32> max_peds             { 0x00E77100 };
        inline util::memory_reference<f32> ped_spawn_radius     { 0x00E770C0 };
        inline util::memory_reference<f32> ped_collide_radius   { 0x00E770C4 };
        inline util::memory_reference<f32> ped_animate_radius   { 0x00E770C8 };
        inline util::memory_reference<f32> ped_anim_move_radius { 0x00E770CC };

        inline util::memory_reference<f32> unk_00e770d0 { 0x00E770D0 }; // with the camera below 0, how far a ped may be from it in height
        inline util::memory_reference<f32> unk_00e770d4 { 0x00E770D4 }; // scales the spawn radius: 1, or 0.5 while the camera is below 0
        inline util::memory_reference<f32> unk_00e770d8 { 0x00E770D8 };
        inline util::memory_reference<i32> unk_00e77104 { 0x00E77104 };

        inline util::memory_reference<i32> unk_0102fee0 { 0x0102FEE0 }; // peds out of cars this frame, spawns included

        // the vendor callback's copies of ped_manager's focus, facing and camera position
        inline util::memory_reference<vector4d> unk_01030a30 { 0x01030A30 };
        inline util::memory_reference<vector4d> unk_01030a40 { 0x01030A40 };
        inline util::memory_reference<vector4d> unk_01030a50 { 0x01030A50 };

        // the fake ped frame's copies for its job: its step, the camera's facing and position
        inline util::memory_reference<f32>      unk_0102fde0 { 0x0102FDE0 };
        inline util::memory_reference<vector4d> unk_01030060 { 0x01030060 };
        inline util::memory_reference<vector4d> unk_01030070 { 0x01030070 };

        // fake peds the job may still place this frame, out of 50
        inline util::memory_reference<u32> unk_0102fde8 { 0x0102FDE8 };
        inline util::memory_reference<u32> unk_0102fdec { 0x0102FDEC };

        inline util::memory_reference<f32> min_fake_ped_radius { 0x00E770A4 }; // set_min_fake_ped_radius writes it
        inline util::memory_reference<f32> unk_00e770b4        { 0x00E770B4 }; // how far fake peds show; the fake ped frame moves it between the inner and outer radius

        inline util::memory_reference<engine_recursive_lock> unk_0102ff80 { 0x0102FF80 }; // replace_fake_ped's

        // spawn_ped searches the one with the larger unk_28, the second on a tie
        inline util::memory_reference<std::array<ped_spawn_quad_search_buffer, 2>> unk_0102ff90 { 0x0102FF90 };

        inline util::memory_reference<string_hash> spawned       { 0x0102CC7C };
        inline util::memory_reference<string_hash> unspawned     { 0x0102C510 };
        inline util::memory_reference<string_hash> vendor_awake  { 0x0102CACC };
        inline util::memory_reference<string_hash> vendor_asleep { 0x0102C794 };
    } // references

    ASSERT_SIZEOF  (ped_path,        0x28);
    ASSERT_OFFSETOF(ped_path, road,  0x04);
    ASSERT_OFFSETOF(ped_path, flags, 0x08);

    ASSERT_SIZEOF  (fake_ped,              0x60);
    ASSERT_OFFSETOF(fake_ped, destination, 0x40);
    ASSERT_OFFSETOF(fake_ped, path,        0x4C);
    ASSERT_OFFSETOF(fake_ped, alpha,       0x50);
    ASSERT_OFFSETOF(fake_ped, fade_step,   0x53);
    ASSERT_OFFSETOF(fake_ped, in_view,     0x57);

    ASSERT_SIZEOF  (ped_spawn_quad_search_buffer,         0x30);
    ASSERT_OFFSETOF(ped_spawn_quad_search_buffer, unk_28, 0x28);
} // treyarch
