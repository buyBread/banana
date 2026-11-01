#pragma once

#include "game/audio/sound_response.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/param_block.hh"
#include "game/als/als.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/resource_key.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mash/array_replacement.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class als_inode;
    class cpu_combat_inode;
    class entity;
    class weapon_interface_extender;
    struct hit_react_record;

    // SM3's eCombatMove, as far as WoS's are read
    enum e_combat_move : i32 {
        combat_move_light_melee
    };

    // SM3's combo_flags_2, as far as WoS's are read
    enum e_combo_flags_2 : u32 {
        combo_flag_sync_anim     = 0x00000008,
        combo_flag_strength_test = 0x00000010,
        combo_flag_buddy_combo   = 0x00200000
    };

    enum e_combo_unk_10_flags : u32 {
        combo_unk_10_flag_00000010 = 0x00000010,
        combo_unk_10_flag_00008000 = 0x00008000, // the hero raises WALL_HIT on what he hits
        combo_unk_10_flag_00010000 = 0x00010000, // AIR_HIT
        combo_unk_10_flag_00020000 = 0x00020000, // GROUND_HIT
        combo_unk_10_flag_00200000 = 0x00200000  // the hero picks up a friendly ai he hits
    };

    enum e_combo_unk_18_flags : u32 {
        combo_unk_18_flag_00000010 = 0x00000010 // the hero's hit adds nothing to meter 0
    };

    // one move of a combat chain
    class combo_move_data : public param_block_creatable_class {

    public:
        u32         flags_1;
        u32         flags_2;
        u32         flags_3;
        u32         unk_10;
        u32         unk_14;
        u32         unk_18;
        u8          reserved_1c[0x04];
        param_block params;

        als::category_id get_target_sync_category(bool* exists) const;
        als::category_id get_buddy_sync_category (bool* exists) const;
        i32              get_combo_move_type_id  (bool* exists) const;
        f32              get_combo_counter_value (bool* exists) const;
        f32              get_combo_counter_delay (bool* exists) const;
    };

    // the melee state; abstract, its subclasses pick the moves
    class combat_state : public enhanced_state {

    public:
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        current_buddies;
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        current_targets;
        u8                              reserved_04c[0x10];
        resource_key                    combat_script_exec_prefix;
        u8                              reserved_064[0x34];
        sound_response                  unk_098;
        u8                              reserved_0a8[0xF8];
        chuck::vm::script_dynamic_array
            <arch_base_vhandle>*        sync_targets;              // the ones start_sync_anim engaged
        arch_base_vhandle               sync_buddy;
        u8                              reserved_1a8[0x2E];
        boolx                           initially_standing;
        u8                              reserved_1d7[0x03];
        boolx                           attack_dodged;
        u8                              reserved_1db[0x01];
        boolx                           attack_blocked;
        u8                              reserved_1dd[0x0D];
        boolx                           in_sync_combo_anim;
        boolx                           in_buddy_combo_anim;
        u8                              reserved_1ec[0x108];
        mash::array_replacement
            <string_hash, 2>            weapon_to_use;

        explicit combat_state(mash::generating_vtable_version so_do_nothing) : enhanced_state(so_do_nothing) {}

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
        bool              is_combat_state() const override;
        u32               unk_019() const override;

        virtual void          update_next_move() = 0;
        virtual e_combat_move get_next_move(f32 delta_t, i32* next_script_move_num) = 0;
        virtual e_combat_move get_initial_move(i32* next_script_move_num);
        virtual void          move_transition(const combo_move_data* new_combo_move, u32 unk);
        virtual void          add_extra_als_params(als_inode* my_als, const combo_move_data* combo_move);
        virtual void          set_als_facing(als_inode* my_als, const combo_move_data* combo_move);
        virtual void          unk_026(als_inode* my_als, const combo_move_data* combo_move);
        virtual void          set_als_position(als_inode* my_als, const combo_move_data* combo_move);

        virtual void update_combat(f32               delta_t,
                                   e_combat_move     next_move_enum,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt);

        virtual void     fill_combat_anim_values(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t);
        virtual bool     is_combo_cancelled(const combo_move_data* combo_move, als_inode* my_als, f32 delta_t) const;
        virtual bool     unk_031(f32 unk);
        virtual void     clear_targets();
        virtual i32      get_damage_type(const combo_move_data* combo_move, arch_base_vhandle target) const;
        virtual i32      get_damage(const combo_move_data* combo_move, arch_base_vhandle target) const;
        virtual vector3d unk_035(const combo_move_data* combo_move, arch_base_vhandle target);
        virtual void     enter_custom_camera(const combo_move_data* combo_move);
        virtual void     exit_custom_camera();
        virtual void     kill_sync_anim(bool next_move_is_synced);
        virtual void     start_sync_anim(const combo_move_data* combo_move);
        virtual void     kill_buddy_combo(bool next_move_is_buddy);
        virtual void     start_buddy_combo(const combo_move_data* combo_move);
        virtual void     kill_strength_test();

        virtual void start_strength_test(const combo_move_data*  combo_move,
                                               als_inode*        my_als,
                                               cpu_combat_inode* cmbt,
                                               f32               delta_t);

        virtual void spawn_sound(const combo_move_data* combo_move,
                                       entity*          player,
                                       string_hash      sound_grp,
                                 const vector3d*        connect_point,
                                       i32              sound_func_id,
                                       void*            unk);

        virtual void spawn_sound_impact(const combo_move_data* combo_move,
                                              entity*          attacker,
                                              entity*          target,
                                        const vector3d*        connect_point,
                                              i32              damage,
                                              bool             blocked);

        virtual string_hash   get_combat_als_category() const;
        virtual als::layer_id get_combat_layer();
        virtual bool          unk_048(const combo_move_data* combo_move, void* unk);

        virtual bool unk_049(      entity*           target,
                             const combo_move_data*  combo_move,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt,
                                   vector3d*         connect_point);

        virtual bool unk_050(      entity*           target,
                             const combo_move_data*  combo_move,
                                   als_inode*        my_als,
                                   cpu_combat_inode* cmbt,
                                   vector3d*         connect_point);

        virtual bool unk_051(entity* target, const vector3d* unk, const vector3d* unk_2);

        virtual bool check_for_target_hit(      entity*           target,
                                          const combo_move_data*  combo_move,
                                                als_inode*        my_als,
                                                cpu_combat_inode* cmbt,
                                                vector3d*         connect_point);

        virtual bool hit_target(      entity*           target,
                                const combo_move_data*  combo_move,
                                      als_inode*        my_als,
                                      cpu_combat_inode* cmbt,
                                      vector3d*         connect_point);

        virtual bool target_was_hit(arch_base_vhandle target);
        virtual bool apply_jump_connect(const combo_move_data* combo_move);
        virtual bool unk_056(const combo_move_data* combo_move);
        virtual void unk_057(const combo_move_data* combo_move, f32 unk);
        virtual void unk_058();
        virtual void unk_059(arch_base_vhandle target);
        virtual void unk_060(f32 unk, const combo_move_data* combo_move);
        virtual void enter_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt);
        virtual void exit_attack_window(const combo_move_data* combo_move, als_inode* my_als, cpu_combat_inode* cmbt);
        virtual void unk_063(als_inode* my_als, entity* target);

        virtual bool check_for_dodge(      entity*                    target,
                                           i32                        chain_size,
                                     const combo_move_data*           combo_move,
                                           weapon_interface_extender* weapon,
                                           entity*                    weapon_entity,
                                           i32                        usage_type,
                                           cpu_combat_inode*          cmbt);

        virtual bool check_for_block(      entity*                    target,
                                           i32                        chain_size,
                                     const combo_move_data*           combo_move,
                                           hit_react_record*          react,
                                           weapon_interface_extender* weapon,
                                           entity*                    weapon_entity,
                                           i32                        usage_type,
                                           cpu_combat_inode*          cmbt);

    private:
        resource_key      get_sync_react_asg(entity* target) const;     // "sync_controlled_combat_react" for the hero, else "sync_combat_react"
        void              release_sync_target(actor* target);
        arch_base_vhandle engage_sync_target(arch_base_vhandle target); // null when it couldn't be engaged
        bool              engage_sync_targets();                        // whether any target is engaged

        void update_initially_standing();
    };

    ASSERT_OFFSETOF(combo_move_data, flags_1, 0x04);
    ASSERT_OFFSETOF(combo_move_data, flags_2, 0x08);
    ASSERT_OFFSETOF(combo_move_data, unk_10,  0x10);
    ASSERT_OFFSETOF(combo_move_data, unk_18,  0x18);
    ASSERT_OFFSETOF(combo_move_data, params,  0x20);

    ASSERT_SIZEOF  (combat_state,                            0x2F8);
    ASSERT_OFFSETOF(combat_state, current_buddies,           0x044);
    ASSERT_OFFSETOF(combat_state, current_targets,           0x048);
    ASSERT_OFFSETOF(combat_state, combat_script_exec_prefix, 0x05C);
    ASSERT_OFFSETOF(combat_state, unk_098,                   0x098);
    ASSERT_OFFSETOF(combat_state, sync_targets,              0x1A0);
    ASSERT_OFFSETOF(combat_state, sync_buddy,                0x1A4);
    ASSERT_OFFSETOF(combat_state, initially_standing,        0x1D6);
    ASSERT_OFFSETOF(combat_state, attack_dodged,             0x1DA);
    ASSERT_OFFSETOF(combat_state, attack_blocked,            0x1DC);
    ASSERT_OFFSETOF(combat_state, in_sync_combo_anim,        0x1EA);
    ASSERT_OFFSETOF(combat_state, in_buddy_combo_anim,       0x1EB);
    ASSERT_OFFSETOF(combat_state, weapon_to_use,             0x2F4);

    inline const string_hash target_sync_layer_key         = string_hash(hash::djb2("target_sync_layer"));
    inline const string_hash use_multiple_sync_targets_key = string_hash(hash::djb2("use_multiple_sync_targets"));

    namespace references {
        // SM3's combat_state::combat_sync_react_asg, "sync_combat_react"
        inline util::memory_reference<resource_key> combat_sync_react_asg { 0x00FC204C };

        // raised on both sides of a sync anim or buddy combo
        inline util::memory_reference<string_hash> ai_combat_sync_begin { 0x0102CB9C };
        inline util::memory_reference<string_hash> ai_combat_sync_end   { 0x0102C674 };
    } // references
} // treyarch
