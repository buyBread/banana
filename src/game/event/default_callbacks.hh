#pragma once

#include "treyarch/mash/string_hash.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch {
    namespace references {
        // static event-type ids, hashed from their names by the CRT initializers
        inline util::memory_reference<string_hash> collide_ragdoll     { 0x0102CB84 };
        inline util::memory_reference<string_hash> footstep_l          { 0x0102C5E4 };
        inline util::memory_reference<string_hash> footstep_r          { 0x0102C6C8 };
        inline util::memory_reference<string_hash> handplant_l         { 0x0102CABC };
        inline util::memory_reference<string_hash> handplant_r         { 0x0102C808 };
        inline util::memory_reference<string_hash> play_sound          { 0x0102CB58 };
        inline util::memory_reference<string_hash> play_sound_on_trans { 0x0102CB64 };
        inline util::memory_reference<string_hash> play_effect         { 0x0102C654 };
        inline util::memory_reference<string_hash> trail_1             { 0x0102C6D4 };
        inline util::memory_reference<string_hash> trail_2             { 0x0102C684 };
        inline util::memory_reference<string_hash> trail_3             { 0x0102C658 };
        inline util::memory_reference<string_hash> trail_4             { 0x0102C754 };
        inline util::memory_reference<string_hash> tentacle_1          { 0x0102C288 };
        inline util::memory_reference<string_hash> tentacle_2          { 0x0102C390 };
        inline util::memory_reference<string_hash> tentacle_3          { 0x0102C2B0 };
        inline util::memory_reference<string_hash> tentacle_4          { 0x0102C720 };
        inline util::memory_reference<string_hash> tentacle_5          { 0x0102C5E0 };
        inline util::memory_reference<string_hash> tentacle_6          { 0x0102CAC0 };
        inline util::memory_reference<string_hash> tentacle_7          { 0x0102C38C };
        inline util::memory_reference<string_hash> tentacle_8          { 0x0102C350 };
        inline util::memory_reference<string_hash> tentacle_9          { 0x0102C284 };
        inline util::memory_reference<string_hash> tentacle_10         { 0x0102CCB4 };
        inline util::memory_reference<string_hash> tentacle_11         { 0x0102C648 };
        inline util::memory_reference<string_hash> tentacle_12         { 0x0102C280 };
        inline util::memory_reference<string_hash> tentacle_13         { 0x0102C5D8 };
        inline util::memory_reference<string_hash> tentacle_14         { 0x0102C5D0 };
        inline util::memory_reference<string_hash> tentacle_15         { 0x0102C4EC };
        inline util::memory_reference<string_hash> tentacle_16         { 0x0102C444 };
        inline util::memory_reference<string_hash> set_vulnerable      { 0x0102C6B0 };
        inline util::memory_reference<string_hash> in_limbo            { 0x0102CAAC };
        inline util::memory_reference<string_hash> particle_event      { 0x0102C7A4 };
        inline util::memory_reference<string_hash> hit_pause           { 0x0102CB78 };
    } // references

    // app::app runs it once, right after the event manager and the other singletons exist
    void register_default_event_callbacks();
} // treyarch
