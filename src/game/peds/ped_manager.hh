#pragma once

#include "vc8/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/math/algebra/vector4d.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    class ped_manager {

    public:
        static void frame_advance(f32 time_inc); // the pedestrians step of the world frame

        // ped_jq's work: spawns a hidden ped's placement, or culls, unspawns and sets up a visible one by distance
        static bool advance_ped(actor*          ped,
                                const vector4d* the_focus,
                                const vector4d* the_camera_position,
                                const vector4d* the_camera_facing);

        static void unspawn_ped(actor* ped); // hides it, resets its ai and suspends it

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

        inline util::memory_reference<string_hash> unspawned { 0x0102C510 };
    } // references
} // treyarch
