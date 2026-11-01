#pragma once

#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/ai/ai_target.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"

namespace treyarch {
    // a move the cpu combat inode lined up for its state, with its targets
    struct cpu_combat_queued_move {
        e_combat_move          move;
        i32                    script_move;
        vc8::vector<ai_target> targets;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    // the ai's melee; its cpu combat inode picks the moves
    class cpu_combat_state : public combat_state {

    public:
        cpu_combat_inode*              unk_2f8;      // the actor's, set by activate
        vc8::list
            <cpu_combat_queued_move*>* queued_moves; // made by activate; update_next_move takes the front one

        explicit cpu_combat_state(mash::generating_vtable_version so_do_nothing) : combat_state(so_do_nothing) {}

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
        void          add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move) override;
    };

    // a civilian beating up another
    class civ_beat_up_cpu_combat_state : public cpu_combat_state {

    public:
        explicit civ_beat_up_cpu_combat_state(mash::generating_vtable_version so_do_nothing) : cpu_combat_state(so_do_nothing) {}

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        void deactivate(const mashed_state* next_mashed_state) override;
        void get_info_node_list(info_node_desc_list &the_list) const override;
    };

    // the melee a slim brain drives
    class generic_slim_combat_state : public combat_state {

    public:
        explicit generic_slim_combat_state(mash::generating_vtable_version so_do_nothing) : combat_state(so_do_nothing) {}

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
        bool              unk_016() const override;

        void          update_next_move() override;
        e_combat_move get_next_move(f32 delta_t, i32* next_script_move_num) override;
        e_combat_move get_initial_move(i32* next_script_move_num) override;
        string_hash   get_combat_als_category() const override;
    };

    ASSERT_SIZEOF  (cpu_combat_queued_move,              0x18);
    ASSERT_OFFSETOF(cpu_combat_queued_move, script_move, 0x04);
    ASSERT_OFFSETOF(cpu_combat_queued_move, targets,     0x08);

    ASSERT_SIZEOF  (cpu_combat_state,               0x300);
    ASSERT_OFFSETOF(cpu_combat_state, unk_2f8,      0x2F8);
    ASSERT_OFFSETOF(cpu_combat_state, queued_moves, 0x2FC);

    ASSERT_SIZEOF(civ_beat_up_cpu_combat_state, 0x300);
    ASSERT_SIZEOF(generic_slim_combat_state,    0x2F8);
} // treyarch
