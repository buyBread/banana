#pragma once

#include "game/arch_base.hh"
#include "game/arch_base_vhandle.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace slim {
        class slim_fight_director_controller;
    } // slim

    class ai_fd_inode;

    enum e_fight_group_flags : u32 {
        fight_group_flag_active = 0x00000001,
        fight_group_flag_paused = 0x00000002
    };

    // one actor of a fight group
    struct fight_group_member {
        arch_base_vhandle actor;
        u32               group_flags; // the script's, set and cleared bit by bit
        bool              unk_008;
        bool              unk_009;
        u8                pad_00a[0x02];
        u32               unk_00c;
    };

    class fight_group : public arch_base {

    public:
        slim::slim_fight_director_controller* controller;
        u32                                   flags;
        vc8::vector<fight_group_member*>      members;
        u8                                    reserved_020[0x10];
        ai_fd_inode*                          fd_inode;

        // inlined @ sub_4CDA20, sub_4CDA70
        fight_group_member* find_member(arch_base_vhandle actor) const {
            for (fight_group_member* member : members) {
                if (member->actor.value == actor.value)
                    return member;
            }

            return nullptr;
        }

        void set_actor_group_flags  (arch_base_vhandle actor, u32 group_flags);
        void clear_actor_group_flags(arch_base_vhandle actor, u32 group_flags);
    };

    ASSERT_SIZEOF  (fight_group_member,              0x10);
    ASSERT_OFFSETOF(fight_group_member, group_flags, 0x04);
    ASSERT_OFFSETOF(fight_group_member, unk_00c,     0x0C);

    ASSERT_SIZEOF  (fight_group,             0x34);
    ASSERT_OFFSETOF(fight_group, controller, 0x08);
    ASSERT_OFFSETOF(fight_group, flags,      0x0C);
    ASSERT_OFFSETOF(fight_group, members,    0x10);
    ASSERT_OFFSETOF(fight_group, fd_inode,   0x30);
} // treyarch
