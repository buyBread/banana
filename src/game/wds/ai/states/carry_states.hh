#pragma once

#include "game/wds/ai/states/sync_anim_state.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the carrier's side of a pick up; deactivating hands the actor to the carry inode
    class pick_up_ai_state : public sync_anim_state {

    public:
        arch_base_vhandle unk_16c; // the actor being picked up, the carry inode's desired slave

        explicit pick_up_ai_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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

        actor* get_slave() const override;
        void   preslave_setup() override;
    };

    // the hero's
    class plr_pick_up_ai_state : public pick_up_ai_state {

    public:
        boolx can_dodge;
        u8    pad_171[0x03];

        explicit plr_pick_up_ai_state(mash::generating_vtable_version so_do_nothing) : pick_up_ai_state(so_do_nothing) {}

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

        e_state_exit_code frame_advance(f32 delta_t) override;

        virtual void unk_034(actor* the_actor);
    };

    // the carrier's side of a put down
    class put_down_ai_state : public sync_anim_state {

    public:
        arch_base_vhandle unk_16c; // the actor being put down
        bool              unk_170; // set unless the carrier was effectively standing as it began
        u8                pad_171[0x03];

        explicit put_down_ai_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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

        actor* get_slave() const override;
        bool   engage_ai_slave_mode() const override;
        void   preslave_setup() override;
    };

    // the hero's
    class plr_put_down_ai_state : public put_down_ai_state {

    public:
        boolx can_dodge;
        u8    pad_175[0x03];

        explicit plr_put_down_ai_state(mash::generating_vtable_version so_do_nothing) : put_down_ai_state(so_do_nothing) {}

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

        virtual void unk_034(actor* the_actor);
    };

    ASSERT_SIZEOF  (pick_up_ai_state,          0x170);
    ASSERT_OFFSETOF(pick_up_ai_state, unk_16c, 0x16C);

    ASSERT_SIZEOF  (plr_pick_up_ai_state,            0x174);
    ASSERT_OFFSETOF(plr_pick_up_ai_state, can_dodge, 0x170);

    ASSERT_SIZEOF  (put_down_ai_state,          0x174);
    ASSERT_OFFSETOF(put_down_ai_state, unk_16c, 0x16C);
    ASSERT_OFFSETOF(put_down_ai_state, unk_170, 0x170);

    ASSERT_SIZEOF  (plr_put_down_ai_state,            0x178);
    ASSERT_OFFSETOF(plr_put_down_ai_state, can_dodge, 0x174);
} // treyarch
