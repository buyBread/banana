#pragma once

#include "game/wds/ai/ai_state_machine.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class game_button;

    // the hero crawling on walls and ceilings
    class plr_loco_crawl_state : public enhanced_state {

    public:
        u32          unk_044; // the id of the actor's ALS_STATE_CHANGE callback
        game_button* unk_048; // map_controls: command button 0
        u8           reserved_04c[0x04];

        explicit plr_loco_crawl_state(mash::generating_vtable_version so_do_nothing) : enhanced_state(so_do_nothing) {}

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

        virtual void map_controls(i32 config);
        virtual void set_player_mode(actor* act);
        virtual bool unk_022();                       // upright within 10 degrees and no more than 1.5 over the ground
        virtual void unk_023(f32 delta_t, bool unk);
    };

    ASSERT_SIZEOF  (plr_loco_crawl_state,          0x50);
    ASSERT_OFFSETOF(plr_loco_crawl_state, unk_044, 0x44);
    ASSERT_OFFSETOF(plr_loco_crawl_state, unk_048, 0x48);
} // treyarch
