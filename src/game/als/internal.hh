#pragma once

#include "game/als/als.hh"
#include "game/als/animation_blend.hh"
#include "game/als/animation_event.hh"
#include "game/als/meta_anim.hh"
#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace als {
    class animation_logic_system;
    class state_machine;

    // what a transition evaluation asks of the state machine
    struct request_data {
        bool     did_transition_occur;
        bool     is_trans_to_category;
        bool     ignore_no_transition;
        bool     unk_03;
        bool     unk_04; // end the layer instead
        u8       pad_05[0x03];
        state_id state_name; // a category's name when is_trans_to_category

        // inlined @ sub_5BD200
        // inlined @ sub_5D6E90
        // inlined @ sub_5D7090
        // inlined @ sub_5D75D0
        // inlined @ sub_5D7780
        void clear() {
            did_transition_occur = false;
            is_trans_to_category = false;
            ignore_no_transition = false;
            unk_03               = false;
            unk_04               = false;
            state_name           = state_id();
        }

        void correct_category_name(actor* the_actor);
    };

    struct state_unk_44 {
        string_hash unk_00;
        f32         unk_04;
        f32         unk_08;
        f32         unk_0c;
        f32         unk_10;
        f32         unk_14;
        u32         unk_18;
        u32         unk_1c;
        f32         unk_20;
        u32         unk_24;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct state_unk_58 {
        string_hash unk_00;
        i32         unk_04;
        u32         unk_08;
        f32         unk_0c;
        f32         unk_10;
        f32         unk_14;
        f32         unk_18;
        f32         unk_1c;
        f32         unk_20;
        f32         unk_24;
        f32         unk_28;
        f32         unk_2c;
        u32         unk_30;
        u32         unk_34;
        u32         unk_38;
        u32         unk_3c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class alignas(8) state : public mash::mash_virtual_base {

    public:
        enum e_flags : u32 {
            uninterruptable          = 0x00000001,
            super_uninterruptable    = 0x00000002,
            animation_reverse        = 0x00000004,
            fail_as_default          = 0x00000008,
            initial_state            = 0x00000010,
            no_blend                 = 0x00000020,
            quick_blend              = 0x00000040,
            medium_blend             = 0x00000080,
            long_blend               = 0x00000100,
            no_blend_out             = 0x00000200,
            quick_blend_out          = 0x00000400,
            medium_blend_out         = 0x00000800,
            long_blend_out           = 0x00001000,
            force_mo_comp_init       = 0x00002000,
            allow_reenter_category   = 0x00004000,
            biped_physics_state      = 0x00008000,
            is_querryable            = 0x00010000,
            biped_phys_no_velocity   = 0x00020000,
            biped_physics_blend_anim = 0x00040000,
            cancel_all_velocity      = 0x00080000,
            initially_suspended      = 0x00100000,
            use_this_layers_mocomp   = 0x00200000,
            cloned_anim              = 0x00400000,
            use_prev_anim_start_time = 0x00800000,
            non_animated             = 0x01000000,
            suspended                = 0x02000000,
            unk_04000000             = 0x04000000, // starting its animation pokes the actor's standing interface
            unk_08000000             = 0x08000000  // change_state keeps the previous state's events and blends
        };

        u8                     pad_004[0x04];
        state_id               name;
        category_id            cat_name;
        u32                    flags; // e_flags
        param_block*           my_param_block;
        mash::vector
            <animation_event>  events;
        mash::vector
            <animation_event>  hit_pause_events; // only HIT_PAUSE ones
        animation_blends_list* blends;
        mash::vector
            <state_unk_44>     unk_44;
        mash::vector
            <state_unk_58>     unk_58;
        mash::vector
            <state_unk_58>     unk_6c;

        state() = default;

        // inlined @ sub_723B40
        // inlined @ sub_723B80
        explicit state(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing),
                                                                        name(so_do_nothing),
                                                                        cat_name(so_do_nothing),
                                                                        events(so_do_nothing),
                                                                        hit_pause_events(so_do_nothing),
                                                                        unk_44(so_do_nothing),
                                                                        unk_58(so_do_nothing),
                                                                        unk_6c(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        // sub_5FD110
        state_id get_state_id() const {
            return name;
        }

        // sub_905E30
        category_id get_cat_id() const {
            return cat_name;
        }

        // the unk_58 record (unk_6c with unk) for the entity's combat id, else for its hit react group, else the unnamed one; past its name
        void* unk_048(bool unk, entity* the_entity) const;

        virtual mash::virtual_types_key get_mocomp_type() const = 0;
        virtual request_data            do_implicit_trans(animation_logic_system* the_als, state_machine* the_sm) const = 0;
        virtual request_data            do_explicit_trans(animation_logic_system* the_als,
                                                          state_machine*          the_sm,
                                                          category_id             desired_category) const = 0;
        virtual nal_anim_id             get_nal_anim_name() const = 0;
        virtual f32                     get_secondary_playback_speed(f32 anim_time, f32 anim_duration) const;
        virtual f32                     get_playback_speed() const;
        virtual f32                     get_start_time() const;

        static string_hash my_type_key;
    };

    class category : public mash::mash_virtual_base {

    public:
        category_id  name;
        param_block* my_param_block;

        category() = default;

        // inlined @ sub_723DA0
        explicit category(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing),
                                                                           name(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        // sub_5FD100
        category_id get_cat_id() const {
            return name;
        }

        virtual request_data do_implicit_trans(animation_logic_system* the_als, state_machine* the_sm) const = 0;
        virtual request_data do_explicit_trans(animation_logic_system* the_als,
                                               state_machine*          the_sm,
                                               category_id             desired_category) const = 0;
        virtual request_data do_incoming_trans(animation_logic_system* the_als,
                                               state_machine*          the_sm,
                                               bool                    unk) const = 0;

        static string_hash my_type_key;
    };

    class transition_group_base : public mash::mash_virtual_base {

    public:
        transition_group_base() = default;

        // inlined @ sub_723DA0
        explicit transition_group_base(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        virtual void placeholder_function() = 0;

        static string_hash my_type_key;
    };

    class state_machine_shared : public mash::mash_virtual_base {

    public:
        mash::vector
            <state>                 state_list;
        mash::vector
            <category>              category_list;
        mash::vector
            <transition_group_base> trans_group_list;

        state_machine_shared() = default;

        // inlined @ sub_723DA0
        explicit state_machine_shared(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing),
                                                                                       state_list(so_do_nothing),
                                                                                       category_list(so_do_nothing),
                                                                                       trans_group_list(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual layer_id get_layer_id() const;

        static string_hash my_type_key;
    };

    class layer_state_machine_shared : public state_machine_shared {

    public:
        u32      bitmask; // layer domains
        layer_id the_layer_id;

        layer_state_machine_shared() = default;

        // inlined @ sub_723DA0
        explicit layer_state_machine_shared(mash::generating_vtable_version so_do_nothing) : state_machine_shared(so_do_nothing) {}

        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        layer_id     get_layer_id() const override;
        virtual void set_layer_id(layer_id new_id);

        static string_hash my_type_key;
    };

    // a type-84 resource: the machines and meta anims every actor playing it shares
    class animation_logic_system_shared {

    public:
        void* self; // the resource handler points this at the root itself

        mash::vector
            <layer_state_machine_shared> the_layers_shared;
        state_machine_shared*            the_base_sm_shared;
        als_meta_anim_table_shared*      meta_anim_table;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (request_data,                       0x0C);
    ASSERT_OFFSETOF(request_data, is_trans_to_category, 0x01);
    ASSERT_OFFSETOF(request_data, ignore_no_transition, 0x02);
    ASSERT_OFFSETOF(request_data, unk_04,               0x04);
    ASSERT_OFFSETOF(request_data, state_name,           0x08);

    ASSERT_SIZEOF(state_unk_44, 0x28);
    ASSERT_SIZEOF(state_unk_58, 0x40);

    ASSERT_SIZEOF  (state,                   0x80);
    ASSERT_OFFSETOF(state, name,             0x08);
    ASSERT_OFFSETOF(state, cat_name,         0x0C);
    ASSERT_OFFSETOF(state, flags,            0x10);
    ASSERT_OFFSETOF(state, my_param_block,   0x14);
    ASSERT_OFFSETOF(state, events,           0x18);
    ASSERT_OFFSETOF(state, hit_pause_events, 0x2C);
    ASSERT_OFFSETOF(state, blends,           0x40);
    ASSERT_OFFSETOF(state, unk_44,           0x44);
    ASSERT_OFFSETOF(state, unk_58,           0x58);
    ASSERT_OFFSETOF(state, unk_6c,           0x6C);

    ASSERT_SIZEOF  (category,                 0x0C);
    ASSERT_OFFSETOF(category, name,           0x04);
    ASSERT_OFFSETOF(category, my_param_block, 0x08);

    ASSERT_SIZEOF(transition_group_base, 0x04);

    ASSERT_SIZEOF  (state_machine_shared,                   0x40);
    ASSERT_OFFSETOF(state_machine_shared, state_list,       0x04);
    ASSERT_OFFSETOF(state_machine_shared, category_list,    0x18);
    ASSERT_OFFSETOF(state_machine_shared, trans_group_list, 0x2C);

    ASSERT_SIZEOF  (layer_state_machine_shared,               0x48);
    ASSERT_OFFSETOF(layer_state_machine_shared, bitmask,      0x40);
    ASSERT_OFFSETOF(layer_state_machine_shared, the_layer_id, 0x44);

    ASSERT_SIZEOF  (animation_logic_system_shared,                     0x20);
    ASSERT_OFFSETOF(animation_logic_system_shared, the_layers_shared,  0x04);
    ASSERT_OFFSETOF(animation_logic_system_shared, the_base_sm_shared, 0x18);
    ASSERT_OFFSETOF(animation_logic_system_shared, meta_anim_table,    0x1C);
}} // treyarch::als
