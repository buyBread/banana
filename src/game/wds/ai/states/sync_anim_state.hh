#pragma once

#include "game/event/generic_event_callback.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/als/als.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class als_inode;

    // a state that plays a synced anim on two actors, the master and the slave
    class sync_anim_state : public attack_enabled_state {

    public:
        als::category_id  master_played_category;
        als::category_id  slave_played_category;
        als::layer_id     played_layer;           // the layer play_anims plays both on
        boolx             engaged_slave_mode;
        u8                pad_161[0x03];
        arch_base_vhandle master;
        arch_base_vhandle slave;

        explicit sync_anim_state(mash::generating_vtable_version so_do_nothing) : attack_enabled_state(so_do_nothing) {}

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

        virtual bool             unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const; // whether cat has played out on the node's layer
        virtual als::category_id get_master_category() const;
        virtual als::category_id get_slave_category() const;
        virtual als::layer_id    get_desired_als_layer() const;
        virtual actor*           get_master() const;
        virtual actor*           get_slave() const = 0;
        virtual bool             engage_ai_slave_mode() const;
        virtual void             play_anims(als::category_id master_cat, als::category_id slave_cat);
        virtual void             preslave_setup();
        virtual bool             unk_033() const;
    };

    class generic_sync_anim_state : public sync_anim_state {

    public:
        explicit generic_sync_anim_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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
        bool             unk_033() const override;
    };

    class ai_obs_hop_sync_state : public generic_sync_anim_state {

    public:
        explicit ai_obs_hop_sync_state(mash::generating_vtable_version so_do_nothing) : generic_sync_anim_state(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        void deactivate(const mashed_state* next_mashed_state) override;
    };

    class generic_slim_sync_anim_state : public generic_sync_anim_state {

    public:
        explicit generic_slim_sync_anim_state(mash::generating_vtable_version so_do_nothing) : generic_sync_anim_state(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;
        void              get_info_node_list(info_node_desc_list &the_list) const override;
        bool              unk_016() const override;
    };

    // the hero's
    class plr_loco_sync_obstacle_avoid_state : public sync_anim_state {

    public:
        u32                    unk_16c;
        e_state_exit_code      unk_170;
        generic_event_callback early_exit_switch_time_callback;
        generic_event_callback nav_exit_switch_time_callback;

        explicit plr_loco_sync_obstacle_avoid_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing),
                                                                                                    early_exit_switch_time_callback(so_do_nothing),
                                                                                                    nav_exit_switch_time_callback(so_do_nothing) {}

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

        bool             unk_024(als_inode* node, als::category_id cat, als::layer_id layer) const override;
        als::category_id get_master_category() const override;
        als::category_id get_slave_category() const override;
        actor*           get_master() const override;
        actor*           get_slave() const override;
        void             preslave_setup() override;

        virtual void unk_034(actor* the_actor);
        virtual void unk_035(u32 unk);
    };

    ASSERT_SIZEOF  (sync_anim_state,                         0x16C);
    ASSERT_OFFSETOF(sync_anim_state, master_played_category, 0x154);
    ASSERT_OFFSETOF(sync_anim_state, played_layer,           0x15C);
    ASSERT_OFFSETOF(sync_anim_state, engaged_slave_mode,     0x160);
    ASSERT_OFFSETOF(sync_anim_state, master,                 0x164);
    ASSERT_OFFSETOF(sync_anim_state, slave,                  0x168);

    ASSERT_SIZEOF(generic_sync_anim_state,      0x16C);
    ASSERT_SIZEOF(ai_obs_hop_sync_state,        0x16C);
    ASSERT_SIZEOF(generic_slim_sync_anim_state, 0x16C);

    ASSERT_SIZEOF  (plr_loco_sync_obstacle_avoid_state,                                  0x184);
    ASSERT_OFFSETOF(plr_loco_sync_obstacle_avoid_state, unk_170,                         0x170);
    ASSERT_OFFSETOF(plr_loco_sync_obstacle_avoid_state, early_exit_switch_time_callback, 0x174);
    ASSERT_OFFSETOF(plr_loco_sync_obstacle_avoid_state, nav_exit_switch_time_callback,   0x17C);

    namespace references {
        // the events plr_loco_sync_obstacle_avoid_state counts
        inline util::memory_reference<string_hash> early_exit_switch_time { 0x0102C63C };
        inline util::memory_reference<string_hash> nav_exit_switch_time   { 0x0102C53C };
    } // references
} // treyarch
