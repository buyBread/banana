#pragma once

#include "game/wds/ai/ai_state_machine.hh"
#include "game/als/als.hh"
#include "game/boolx.hh"
#include "game/float_spring.hh"
#include "game/hash/algo.hh"
#include "game/resource_key.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class game_button;

    // the hero on foot
    class plr_loco_standing_state : public attack_enabled_state {

    public:
        f32          unk_154;               // the hero's speed across the ground as it began
        vector3d     unk_158;               // its z facing as it began
        vector3d     unk_164;               // its physical interface's +0x20 as it began; the landing's fall speed is its y negated
        bool         unk_170;
        u8           pad_171[0x03];
        f32          unk_174;               // slot 21 hands it to the base layer as meta_linear_blend_amount
        f32          unk_178;
        f32          unk_17c;               // three timers slot 28 steps
        f32          unk_180;
        f32          unk_184;
        float_spring unk_188;               // slot 21 steps it
        f32          unk_1a4;
        boolx        can_pickup_putdown;
        boolx        can_attack;
        boolx        can_dodge;
        boolx        can_hit_switch;
        boolx        can_hit_ranged_switch;
        boolx        can_crawl;
        boolx        did_anim_request;
        boolx        initial_startup;       // set by an activate without a previous state
        bool         unk_1b0;               // landing; get_als_category answers "Fall_Land"
        bool         unk_1b1;
        u8           pad_1b2[0x02];
        i32          unk_1b4;
        f32          unk_1b8;               // an angle in degrees
        i32          unk_1bc;               // 1 while unk_1b8 is within 20 degrees, else 2 below and 3 above
        f32          unk_1c0;
        f32          unk_1c4;
        u32          unk_1c8;
        f32          unk_1cc;               // how long the crawl entry has been waiting
        u32          unk_1d0;
        f32          unk_1d4;
        game_button* zip_button;
        game_button* wall_sprint_button;
        game_button* yoyo_button;

        explicit plr_loco_standing_state(mash::generating_vtable_version so_do_nothing) : attack_enabled_state(so_do_nothing) {}

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;
        void              get_info_node_list(info_node_desc_list &the_list) const override;

        i32 unk_021(f32 unk) override;

        virtual als::category_id  get_als_category() const;
        virtual bool              unk_025();
        virtual void              map_controls(i32 config);
        virtual void              set_player_mode(actor* act);
        virtual void              unk_028(f32 delta_t);
        virtual void              unk_029(f32 delta_t);
        virtual e_state_exit_code unk_030(f32 delta_t);
        virtual void              unk_031(f32 delta_t);
        virtual void              unk_032(i32 unk, void* unk_2);
        virtual void              unk_033(f32 unk, f32 unk_2, f32 unk_3);
    };

    inline const string_hash delay_standing_frames_key = string_hash(hash::djb2("delay_standing_frames"));

    ASSERT_SIZEOF  (plr_loco_standing_state,                     0x1E4);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_164,            0x164);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_174,            0x174);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_1a4,            0x1A4);
    ASSERT_OFFSETOF(plr_loco_standing_state, can_pickup_putdown, 0x1A8);
    ASSERT_OFFSETOF(plr_loco_standing_state, can_crawl,          0x1AD);
    ASSERT_OFFSETOF(plr_loco_standing_state, initial_startup,    0x1AF);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_1b0,            0x1B0);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_1b4,            0x1B4);
    ASSERT_OFFSETOF(plr_loco_standing_state, unk_1cc,            0x1CC);
    ASSERT_OFFSETOF(plr_loco_standing_state, zip_button,         0x1D8);
    ASSERT_OFFSETOF(plr_loco_standing_state, yoyo_button,        0x1E0);

    namespace references {
        // "sync_controlled_combat_react"; the standing state's own copy
        inline util::memory_reference<resource_key> unk_00fc6bc0 { 0x00FC6BC0 };
    } // references
} // treyarch
