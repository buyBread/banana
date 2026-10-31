#pragma once

#include "game/wds/ai/states/sync_anim_state.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_civilian_agressor_inode;

    // the attacker's states over its victim; each couples the victim's als to the attacker's as it begins
    class hold_hostage_state : public sync_anim_state {

    public:
        bool unk_16c;
        bool unk_16d;
        bool unk_16e;
        u8   pad_16f;
        f32  unk_170;
        bool unk_174;
        u8   pad_175[0x03];

        explicit hold_hostage_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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
    };

    class harass_victim_state : public sync_anim_state {

    public:
        explicit harass_victim_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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

        actor* get_slave() const override;
    };

    class hold_abductee_state : public sync_anim_state {

    public:
        u32  unk_16c;
        u32  unk_170;
        f32  unk_174; // 0.75 to 2.25, picked as it begins
        bool unk_178;
        u8   pad_179[0x03];

        explicit hold_abductee_state(mash::generating_vtable_version so_do_nothing) : sync_anim_state(so_do_nothing) {}

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
    };

    ASSERT_SIZEOF  (hold_hostage_state,          0x178);
    ASSERT_OFFSETOF(hold_hostage_state, unk_170, 0x170);
    ASSERT_OFFSETOF(hold_hostage_state, unk_174, 0x174);

    ASSERT_SIZEOF(harass_victim_state, 0x16C);

    ASSERT_SIZEOF  (hold_abductee_state,          0x17C);
    ASSERT_OFFSETOF(hold_abductee_state, unk_174, 0x174);
    ASSERT_OFFSETOF(hold_abductee_state, unk_178, 0x178);

    namespace references {
        // what the activates hand the voice box, made on first use
        inline util::memory_reference<string_hash> hold_hostage_line        { 0x00FC515C };
        inline util::memory_reference<u32>         hold_hostage_line_guard  { 0x00FC5160 };
        inline util::memory_reference<string_hash> harass_victim_line       { 0x00FC5164 };
        inline util::memory_reference<u32>         harass_victim_line_guard { 0x00FC5168 };
        inline util::memory_reference<string_hash> abduct_line              { 0x00FC516C };
        inline util::memory_reference<u32>         abduct_line_guard        { 0x00FC5170 };
    } // references
} // treyarch
