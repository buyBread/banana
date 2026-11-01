#pragma once

#include "vc8/hash_map.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_root_inode;
    class param_block;

    // the flags source reads by index; the name map knows every one by its name
    enum e_ai_flag : i32 {
        ai_flag_is_inside_car                = 0x001,
        ai_flag_ai_targetable                = 0x00B,
        ai_flag_allow_movement_during_attack = 0x05F,
        ai_flag_do_capture                   = 0x146,
        ai_flag_frenzy_mode                  = 0x18D,
        ai_flag_is_slim_actor                = 0x21A,
        ai_flag_pod_is_off_base              = 0x289,
        ai_flag_can_convert                  = 0x3C3,
        ai_flag_currently_in_car             = 0x3C7,
        ai_flag_allowed_to_tactics           = 0x3F3
    };

    // an ai_root_inode's flags; flag n is bit n & 31 of bits[n >> 5]
    class ai_flag_store {

    public:
        engine_recursive_lock lock;
        u32                   bits[34];
        bool                  initialized;
        u8                    pad_099[0x03];
        ai_root_inode*        owner;

        void initialize(); // filled from the owner's generic actor defaults, then its vars
        void apply_params(param_block* params);
        void set(i32 flag, bool value);
        bool get(i32 flag);
    };

    ASSERT_SIZEOF  (ai_flag_store,              0xA0);
    ASSERT_OFFSETOF(ai_flag_store, bits,        0x10);
    ASSERT_OFFSETOF(ai_flag_store, initialized, 0x98);
    ASSERT_OFFSETOF(ai_flag_store, owner,       0x9C);

    namespace references {
        // every flag's name hash and index, built on first use
        inline util::memory_reference<vc8::hash_map<u32, i32>> ai_flag_name_map { 0x00FC335C };

        inline util::memory_reference<u32> ai_flag_name_map_guard { 0x00FC3388 };
    } // references

    vc8::hash_map<u32, i32>* get_ai_flag_name_map();
    i32                      find_ai_flag(const vc8::hash_map<u32, i32>* flag_name_map, u32 name_hash, i32 unused); // -1 when the name isn't a flag

    // the ai flag a name's hash stands for, -1 when there's none
    inline i32 find_ai_flag(u32 name_hash) {
        return find_ai_flag(get_ai_flag_name_map(), name_hash, 1);
    }
} // treyarch
