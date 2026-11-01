#pragma once

#include "game/wds/ai/states/combat_state.hh"
#include "game/boolx.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    class ai_target;
    class als_inode;
    class game_button;
    class player_interface;

    // one of player_interface's attacks
    class plr_combat_attack : public mash::mash_virtual_base {

    public:
        const game_button*      button;
              f32               timer;                       // -1 while no press is pending
              vector3d          attack_dir;
              vector3d          attack_up;
              i32               script_move_num;             // the force paths store it; plr_combat_state hands it on as the next script move
              vc8::vector
                  <ai_target>*  targets;
              u32               target_flags;
              f32               min_dotp;
              f32               min_vert_dotp;
              f32               min_range;
              f32               max_range;
              f32               min_vert_range;
              f32               max_vert_range;
              boolx             allow_no_targets;
              boolx             button_held;
              boolx             inside_min_range_ignore_arc;
              boolx             unk_4b;                      // set, update takes a press that ties the best time

        plr_combat_attack();
        ~plr_combat_attack() override;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual bool     update(e_combat_move            move,
                                const player_interface*  plr_ifc,
                                als_inode*               my_als,
                                f32                      delta_t,
                                f32*                     best_time,
                                e_combat_move            current_chosen_move,
                                const plr_combat_attack* current_chosen_attack,
                                const void*              unk);
        virtual void     clear();
        virtual bool     has_potential_targets(const player_interface* plr_ifc, bool unk);
        virtual bool     build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool unk_4);
        virtual bool     update_target_list(const player_interface* plr_ifc);
        virtual bool     should_execute(const player_interface* plr_ifc, f32 tolerance);
        virtual void     forced_set_up(const player_interface* plr_ifc, e_combat_move move, const ai_target* targ, i32 script_move);
        virtual void     forced_set_up(const player_interface* plr_ifc, e_combat_move move, const vc8::vector<ai_target>* targs, i32 script_move);
        virtual vector3d get_attack_direction(const player_interface* plr_ifc, const void* unk);
        virtual vector3d get_attack_up(const player_interface* plr_ifc, const void* unk);
        virtual bool     attack_possible(const player_interface* plr_ifc);
        virtual f32      compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override);
        virtual bool     target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc);
        virtual f32      unk_022(e_combat_move move, const player_interface* plr_ifc, als_inode* my_als); // the als's combat_button_begin_time
        virtual f32      unk_023(e_combat_move move, const player_interface* plr_ifc, als_inode* my_als); // the als's combat_button_end_time

        void initialize(mash::allocation_scope scope);
    };

    // one of player_interface's tricks
    class plr_trick_move : public mash::mash_virtual_base {

    public:
        const game_button* button;
              f32          timer;
              boolx        button_held;
              u8           pad_0d[0x03];

        plr_trick_move();
        ~plr_trick_move() override;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual bool update(f32 delta_t, f32* best_time, bool has_next_trick);
        virtual void clear();
        virtual bool should_execute() const;

        void init(const game_button* _button);
    };

    ASSERT_SIZEOF  (plr_combat_attack,                   0x4C);
    ASSERT_OFFSETOF(plr_combat_attack, button,           0x04);
    ASSERT_OFFSETOF(plr_combat_attack, attack_dir,       0x0C);
    ASSERT_OFFSETOF(plr_combat_attack, script_move_num,  0x24);
    ASSERT_OFFSETOF(plr_combat_attack, targets,          0x28);
    ASSERT_OFFSETOF(plr_combat_attack, min_dotp,         0x30);
    ASSERT_OFFSETOF(plr_combat_attack, allow_no_targets, 0x48);

    ASSERT_SIZEOF  (plr_trick_move,              0x10);
    ASSERT_OFFSETOF(plr_trick_move, timer,       0x08);
    ASSERT_OFFSETOF(plr_trick_move, button_held, 0x0C);
} // treyarch
