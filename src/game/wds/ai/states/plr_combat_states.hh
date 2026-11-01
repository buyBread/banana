#pragma once

#include "game/wds/ai/states/combat_state.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the hero's melee
    class plr_combat_state : public combat_state {

    public:
        u8 reserved_2f8[0x0C];

        explicit plr_combat_state(mash::generating_vtable_version so_do_nothing) : combat_state(so_do_nothing) {}

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

        void          update_next_move() override;
        e_combat_move get_next_move(f32 delta_t, i32* next_script_move_num) override;
        e_combat_move get_initial_move(i32* next_script_move_num) override;
        void          move_transition(const combo_move_data* new_combo_move, u32 unk) override;
        void          add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) override;
        void          fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) override;
        void          clear_targets() override;
        i32           get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const override;
        i32           get_damage(const combo_move_data* combo_move, arch_base_vhandle target) const override;
        vector3d      unk_035(const combo_move_data* combo_move, arch_base_vhandle target) override;
        bool          unk_048(const combo_move_data* combo_move, void* unk) override;

        bool hit_target(      entity*           target,
                        const combo_move_data*  combo_move,
                              als_inode*        my_als,
                              cpu_combat_inode* cmbt,
                              vector3d*         connect_point) override;

        bool apply_jump_connect(const combo_move_data* combo_move) override;

        bool check_for_block(      entity*                    target,
                                   i32                        chain_size,
                             const combo_move_data*           combo_move,
                                   hit_react_record*          react,
                                   weapon_interface_extender* weapon,
                                   entity*                    weapon_entity,
                                   i32                        usage_type,
                                   cpu_combat_inode*          cmbt) override;

        virtual void unk_066(actor* the_actor); // the player interface's mode word becomes 9
    };

    // the hero's melee on a car
    class plr_car_combat_attack_state : public plr_combat_state {

    public:
        explicit plr_car_combat_attack_state(mash::generating_vtable_version so_do_nothing) : plr_combat_state(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;
    };

    class rhino_ride_state : public plr_combat_state {

    public:
        u8 reserved_304[0x14];

        explicit rhino_ride_state(mash::generating_vtable_version so_do_nothing) : plr_combat_state(so_do_nothing) {}

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
    };

    // the hero's tricks
    class plr_trick_state : public combat_state {

    public:
        u8 reserved_2f8[0x20];

        explicit plr_trick_state(mash::generating_vtable_version so_do_nothing) : combat_state(so_do_nothing) {}

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

        void          update_next_move() override;
        e_combat_move get_next_move(f32 delta_t, i32* next_script_move_num) override;
        e_combat_move get_initial_move(i32* next_script_move_num) override;
        void          move_transition(const combo_move_data* new_combo_move, u32 unk) override;
        void          add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) override;
        void          fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) override;

        virtual void unk_066(actor* the_actor); // the player interface's mode word becomes 14
    };

    ASSERT_SIZEOF(plr_combat_state,            0x304);
    ASSERT_SIZEOF(plr_car_combat_attack_state, 0x304);
    ASSERT_SIZEOF(rhino_ride_state,            0x318);
    ASSERT_SIZEOF(plr_trick_state,             0x318);

    namespace references {
        // what the hero raises on himself as his move hits
        inline util::memory_reference<string_hash> ground_hit { 0x0102C364 };
        inline util::memory_reference<string_hash> wall_hit   { 0x0102CAE4 };
        inline util::memory_reference<string_hash> air_hit    { 0x0102C6F0 };

        // 0.1, the share of a hit's dmg_info.unk_38 the combo meter gains
        inline util::memory_reference<f32> unk_00bb217c { 0x00BB217C };

        // 10, the combo counter's delay in frames when the move has none
        inline util::memory_reference<i32> unk_00bb2184 { 0x00BB2184 };
    } // references
} // treyarch
