#pragma once

#include "game/wds/ai/states/sync_anim_state.hh"
#include "game/arch_base_vhandle.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the state's own actor is the slave
    class enter_car_state : public sync_anim_state {

    public:
        explicit enter_car_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;
        void              get_info_node_list(info_node_desc_list &the_list) const override;

        bool             unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const override;
        als::category_id get_master_category() const override;
        als::category_id get_slave_category() const override;
        als::layer_id    get_desired_als_layer() const override;
        actor*           get_master() const override;
        actor*           get_slave() const override;
    };

    // the state's own actor is the slave
    class exit_car_state : public sync_anim_state {

    public:
        arch_base_vhandle selected_car;  // the car inode's, as activate found them
        i32               selected_seat;

        explicit exit_car_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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

        void deactivate(const mashed_state* next_mashed_state) override;
        void get_info_node_list(info_node_desc_list &the_list) const override;

        bool             unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const override;
        als::category_id get_master_category() const override;
        als::category_id get_slave_category() const override;
        als::layer_id    get_desired_als_layer() const override;
        actor*           get_master() const override;
        actor*           get_slave() const override;
    };

    ASSERT_SIZEOF(enter_car_state, 0x16C);

    ASSERT_SIZEOF  (exit_car_state,                0x174);
    ASSERT_OFFSETOF(exit_car_state, selected_car,  0x16C);
    ASSERT_OFFSETOF(exit_car_state, selected_seat, 0x170);
} // treyarch
