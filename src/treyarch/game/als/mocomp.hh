#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class actor;

namespace als {
    class animation_logic_system;
    class state_machine;

    // what a layer's state can say about the anim's offset
    inline const string_hash mocomp_ignore_positional_offset_id  = string_hash(hash::djb2("mocomp_ignore_positional_offset"));
    inline const string_hash mocomp_allow_knockback_scale_id     = string_hash(hash::djb2("mocomp_allow_knockback_scale"));
    inline const string_hash mocomp_allow_knockup_scale_id       = string_hash(hash::djb2("mocomp_allow_knockup_scale"));
    inline const string_hash mocomp_allow_knockdown_scale_id     = string_hash(hash::djb2("mocomp_allow_knockdown_scale"));
    inline const string_hash mocomp_allow_air_knockback_scale_id = string_hash(hash::djb2("mocomp_allow_air_knockback_scale"));
    inline const string_hash mocomp_allow_air_knockup_scale_id   = string_hash(hash::djb2("mocomp_allow_air_knockup_scale"));
    inline const string_hash mocomp_allow_air_knockdown_scale_id = string_hash(hash::djb2("mocomp_allow_air_knockdown_scale"));

    // moves and turns the actor the way its animation asks; built in the als's own storage
    class motion_compensator : public mash::mash_virtual_base {

    public:
        animation_logic_system* the_als;
        state_machine*          the_base;
        actor*                  the_actor;

        motion_compensator() = default;

        // inlined @ sub_723DA0
        explicit motion_compensator(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual void     activate(animation_logic_system* the_als_core, layer_id layer);
        virtual void     deactivate();
        virtual void     transition();
        virtual void     pre_anim_action(f32 delta_t);
        virtual void     post_anim_action(f32 delta_t);
        virtual bool     is_request_fulfilled();
        virtual bool     force_reactivate() const;
        virtual bool     unk_016() const;
        virtual layer_id get_layer_id() const;
        virtual void     get_po_curr_offset(po &curr_offset); // the anim's trajectory this frame, scaled as the state allows
        virtual void     set_po_curr_offset(actor* target, po &curr_offset);

        // builds the class in the buffer; null for a key it doesn't know
        static motion_compensator* create_subclass_in_place(mash::virtual_types_key the_class_to_make, void* memory_buffer);

        static string_hash my_type_key;
        static i32         last_created_size; // never read
    };

    // applies the anim's offset as it is
    class use_anim_only : public motion_compensator {

    public:
        use_anim_only() = default;

        // inlined @ sub_723DA0
        explicit use_anim_only(mash::generating_vtable_version so_do_nothing) : motion_compensator(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void post_anim_action(f32 delta_t) override;

        static string_hash my_type_key;
    };

    inline const string_hash biped_physics_mocomp_key = string_hash(hash::djb2("als::biped_physics_mocomp"));

    ASSERT_SIZEOF  (motion_compensator,            0x10);
    ASSERT_OFFSETOF(motion_compensator, the_als,   0x04);
    ASSERT_OFFSETOF(motion_compensator, the_base,  0x08);
    ASSERT_OFFSETOF(motion_compensator, the_actor, 0x0C);

    ASSERT_SIZEOF(use_anim_only, 0x10);
}} // treyarch::als
