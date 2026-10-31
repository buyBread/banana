#pragma once

#include "game/wds/ai/actions/ai_action_nugget.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class std_carry_inode;

    // the hero picking up an ai: the target plays its side on layer 9 and ends up as the carry inode's slave
    class pick_up_ai_action : public play_anim_ai_action {

    public:
        arch_base_vhandle target;
        boolx             finished;      // the target was handed to the carry inode
        boolx             engaged_slave; // activate put the target into slave mode
        bool              unk_026;       // frame_advance finishes while it's set
        u8                pad_027[0x01];

        pick_up_ai_action(ai_core* owner, const string_hash &my_hash = string_hash());

        e_action_exit_status frame_advance(f32 delta_t) override;
        void                 activate() override;
        void                 deactivate(bool on_finalize) override;

        virtual void setup(std_carry_inode* _slave);
    };

    ASSERT_SIZEOF  (pick_up_ai_action,                0x28);
    ASSERT_OFFSETOF(pick_up_ai_action, target,        0x20);
    ASSERT_OFFSETOF(pick_up_ai_action, finished,      0x24);
    ASSERT_OFFSETOF(pick_up_ai_action, engaged_slave, 0x25);
    ASSERT_OFFSETOF(pick_up_ai_action, unk_026,       0x26);
} // treyarch
