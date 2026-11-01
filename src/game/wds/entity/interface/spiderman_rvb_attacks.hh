#pragma once

#include "game/wds/entity/interface/plr_combat_attack.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class als_inode;
    class player_interface;

    class spiderman_rvb_grapple_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_grapple_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        bool attack_possible(const player_interface* plr_ifc) override;
        f32  compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) override;
    };

    class spiderman_rvb_air_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_air_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_air_jump_attack : public spiderman_rvb_air_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_air_jump_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_car_finish_attack : public plr_combat_attack {

    public:
        arch_base_vhandle combat_vehicle;

        static string_hash my_type_key;

        spiderman_rvb_car_finish_attack();
        ~spiderman_rvb_car_finish_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        void clear() override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) override;

    private:
        void initialize(mash::allocation_scope scope);
    };

    class spiderman_rvb_throw_manip_attack : public plr_combat_attack {

    public:
        arch_base_vhandle manip_to_throw;

        static string_hash my_type_key;

        spiderman_rvb_throw_manip_attack();
        ~spiderman_rvb_throw_manip_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        void clear() override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
        bool target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) override;

    private:
        void initialize(mash::allocation_scope scope);
    };

    class spiderman_rvb_web_yank_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_web_yank_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_melee_or_car_combat_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_melee_or_car_combat_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
        f32  compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) override;
        bool target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_momentum_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_momentum_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool has_potential_targets(const player_interface* plr_ifc, bool check_possible) override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
        f32  compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) override;
    };

    class spiderman_rvb_civ_save_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_civ_save_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool has_potential_targets(const player_interface* plr_ifc, bool check_possible) override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
        f32  compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) override;
    };

    class spiderman_rvb_wall_attack : public plr_combat_attack {

    public:
        u8       reserved_4c[0x0C];
        vector3d unk_58;            // update faces the owner along its negation

        static string_hash my_type_key;

        ~spiderman_rvb_wall_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_pole_swing_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_pole_swing_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool     update(e_combat_move            move,
                        const player_interface*  plr_ifc,
                        als_inode*               my_als,
                        f32                      delta_t,
                        f32*                     best_time,
                        e_combat_move            current_chosen_move,
                        const plr_combat_attack* current_chosen_attack,
                        const void*              unk) override;
        vector3d get_attack_direction(const player_interface* plr_ifc, const void* unk) override;
        bool     attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_swing_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_swing_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_melee_or_car_combat_ground_attack : public spiderman_rvb_melee_or_car_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_melee_or_car_combat_ground_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_special_attack : public spiderman_rvb_melee_or_car_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_special_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;

    protected:
        bool attack_possible_with_upgrades(const player_interface* plr_ifc, const char* black_upgrade, const char* red_upgrade); // black mode needs the black upgrade, red mode the red one
    };

    class spiderman_rvb_special_x_attack : public spiderman_rvb_special_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_special_x_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_special_y_attack : public spiderman_rvb_special_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_special_y_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_special_b_attack : public spiderman_rvb_special_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_special_b_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool attack_possible(const player_interface* plr_ifc) override;
    };

    class spiderman_rvb_counter_attack : public plr_combat_attack {

    public:
        static string_hash my_type_key;

        ~spiderman_rvb_counter_attack() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool update(e_combat_move            move,
                    const player_interface*  plr_ifc,
                    als_inode*               my_als,
                    f32                      delta_t,
                    f32*                     best_time,
                    e_combat_move            current_chosen_move,
                    const plr_combat_attack* current_chosen_attack,
                    const void*              unk) override;
        bool build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) override;
        bool attack_possible(const player_interface* plr_ifc) override;
    };

    ASSERT_SIZEOF(spiderman_rvb_grapple_attack,                    0x4C);
    ASSERT_SIZEOF(spiderman_rvb_air_attack,                        0x4C);
    ASSERT_SIZEOF(spiderman_rvb_air_jump_attack,                   0x4C);
    ASSERT_SIZEOF(spiderman_rvb_car_finish_attack,                 0x50);
    ASSERT_SIZEOF(spiderman_rvb_throw_manip_attack,                0x50);
    ASSERT_SIZEOF(spiderman_rvb_web_yank_attack,                   0x4C);
    ASSERT_SIZEOF(spiderman_rvb_melee_or_car_combat_attack,        0x4C);
    ASSERT_SIZEOF(spiderman_rvb_momentum_attack,                   0x4C);
    ASSERT_SIZEOF(spiderman_rvb_civ_save_attack,                   0x4C);
    ASSERT_SIZEOF(spiderman_rvb_wall_attack,                       0x64);
    ASSERT_SIZEOF(spiderman_rvb_pole_swing_attack,                 0x4C);
    ASSERT_SIZEOF(spiderman_rvb_swing_attack,                      0x4C);
    ASSERT_SIZEOF(spiderman_rvb_melee_or_car_combat_ground_attack, 0x4C);
    ASSERT_SIZEOF(spiderman_rvb_special_attack,                    0x4C);
    ASSERT_SIZEOF(spiderman_rvb_special_x_attack,                  0x4C);
    ASSERT_SIZEOF(spiderman_rvb_special_y_attack,                  0x4C);
    ASSERT_SIZEOF(spiderman_rvb_special_b_attack,                  0x4C);
    ASSERT_SIZEOF(spiderman_rvb_counter_attack,                    0x4C);

    ASSERT_OFFSETOF(spiderman_rvb_car_finish_attack,  combat_vehicle, 0x4C);
    ASSERT_OFFSETOF(spiderman_rvb_throw_manip_attack, manip_to_throw, 0x4C);
    ASSERT_OFFSETOF(spiderman_rvb_wall_attack,        unk_58,         0x58);
} // treyarch
