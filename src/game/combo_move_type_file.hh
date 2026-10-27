#pragma once

#include "game/boolx.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every move; mashed ones get their retail vtable from the type registry
    class combo_move_track_info : public mash::mash_virtual_base {

    public:
        i32 move_id;
        u8  unk_08[0x18];

        // sub_90AAC0
        static i32 compare_move_id(const void* left, const void* right);
    };

    class combat_move_track_info : public combo_move_track_info {

    public:
        f32 spider_meter_amount;
        f32 damage_percentage;
    };

    class combo_move_type_file {

    public:
        void* self; // the resource handler points this at the file itself

        mash::vector
            <combat_move_track_info> combo_move_info_array;
        mash::vector
            <combo_move_track_info>  trick_move_info_array;

        i32   max_combo_chain;
        boolx use_trick_degradation;
        boolx use_chain_deg_window;
        boolx combat_apply_damage_percent;
        boolx use_time_combo_breaker;
        f32   time_breaker_duration;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (combo_move_track_info,          0x20);
    ASSERT_OFFSETOF(combo_move_track_info, move_id, 0x04);

    ASSERT_SIZEOF  (combat_move_track_info,                      0x28);
    ASSERT_OFFSETOF(combat_move_track_info, spider_meter_amount, 0x20);

    ASSERT_SIZEOF  (combo_move_type_file,                         0x38);
    ASSERT_OFFSETOF(combo_move_type_file, trick_move_info_array,  0x18);
    ASSERT_OFFSETOF(combo_move_type_file, max_combo_chain,        0x2C);
    ASSERT_OFFSETOF(combo_move_type_file, use_trick_degradation,  0x30);
    ASSERT_OFFSETOF(combo_move_type_file, use_time_combo_breaker, 0x33);
    ASSERT_OFFSETOF(combo_move_type_file, time_breaker_duration,  0x34);
} // treyarch
