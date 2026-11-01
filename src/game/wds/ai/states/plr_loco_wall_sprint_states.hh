#pragma once

#include "game/event/generic_event_callback.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/boolx.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class spline;

    // the hero running along a wall
    class plr_loco_wall_sprint_base_state : public enhanced_state {

    public:
        boolx can_attack;
        boolx can_dodge;
        boolx can_crawl;
        boolx can_jump;

        explicit plr_loco_wall_sprint_base_state(mash::generating_vtable_version so_do_nothing) : enhanced_state(so_do_nothing) {}

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

        virtual void     map_controls(i32 config);
        virtual void     set_player_mode(actor* act);
        virtual vector3d get_z_facing() const;      // the actor's
        virtual vector3d get_y_facing() const;
        virtual vector3d get_x_facing() const;
    };

    // getting onto a wall and off it again
    class plr_loco_wall_sprint_transition_state : public plr_loco_wall_sprint_base_state {

    public:
        u8                     reserved_048[0x0C];
        vector3d               unk_054;               // the hero's velocity as it began; a type 10 transition leaves with it
        f32                    unk_060;
        f32                    unk_064;
        bool                   unk_068;
        bool                   unk_069;
        u8                     reserved_06a[0x06];
        spline*                path;                  // built by activate
        generic_event_callback web_shoot_callback;
        generic_event_callback web_release_callback;
        u32                    unk_084[2];            // the webs slot 26 shoots and slot 27 kills

        explicit plr_loco_wall_sprint_transition_state(mash::generating_vtable_version so_do_nothing) : plr_loco_wall_sprint_base_state(so_do_nothing),
                                                                                                        web_shoot_callback(so_do_nothing),
                                                                                                        web_release_callback(so_do_nothing) {}

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

        virtual void unk_025();
        virtual void unk_026(); // shoots the webs the callbacks asked for
        virtual void unk_027(); // kills both webs
    };

    ASSERT_SIZEOF  (plr_loco_wall_sprint_base_state,             0x48);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_base_state, can_attack, 0x44);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_base_state, can_jump,   0x47);

    ASSERT_SIZEOF  (plr_loco_wall_sprint_transition_state,                       0x8C);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, unk_054,              0x54);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, unk_068,              0x68);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, path,                 0x70);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, web_shoot_callback,   0x74);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, web_release_callback, 0x7C);
    ASSERT_OFFSETOF(plr_loco_wall_sprint_transition_state, unk_084,              0x84);
} // treyarch
