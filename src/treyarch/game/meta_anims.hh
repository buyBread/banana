#pragma once

#include "treyarch/game/als/meta_anim.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

// the game's own filename meta anims; each picks the anim to play for the actor
namespace treyarch {
    class actor;

    class generic_anim_by_filename : public als::als_meta_anim_by_filename {

    public:
        generic_anim_by_filename() = default;

        // sub_71C670
        explicit generic_anim_by_filename(mash::generating_vtable_version so_do_nothing) : als_meta_anim_by_filename(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        nal_anim_id get_anim_id_to_play(actor* the_actor) override;

        static string_hash my_type_key;
    };

    class ai_puppet_anim_by_filename : public als::als_meta_anim_by_filename {

    public:
        ai_puppet_anim_by_filename() = default;

        // sub_71B9C0
        explicit ai_puppet_anim_by_filename(mash::generating_vtable_version so_do_nothing) : als_meta_anim_by_filename(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        nal_anim_id get_anim_id_to_play(actor* the_actor) override;

        static string_hash my_type_key;
    };

    class combat_dodge_meta_anim : public als::als_meta_anim_by_filename {

    public:
        mash::vector
            <als::meta_key_anim> key_anims;
        u32                      unk_4c;

        combat_dodge_meta_anim() = default;

        // sub_7204D0
        explicit combat_dodge_meta_anim(mash::generating_vtable_version so_do_nothing) : als_meta_anim_by_filename(so_do_nothing),
                                                                                         key_anims(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        nal_anim_id get_anim_id_to_play(actor* the_actor) override;

        static string_hash my_type_key;
    };

    class combat_block_meta_anim : public als::als_meta_anim_by_filename {

    public:
        mash::vector
            <als::meta_key_anim> key_anims;
        u32                      unk_4c;

        combat_block_meta_anim() = default;

        // sub_7205D0
        explicit combat_block_meta_anim(mash::generating_vtable_version so_do_nothing) : als_meta_anim_by_filename(so_do_nothing),
                                                                                         key_anims(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        nal_anim_id get_anim_id_to_play(actor* the_actor) override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF(generic_anim_by_filename,   0x38);
    ASSERT_SIZEOF(ai_puppet_anim_by_filename, 0x38);

    ASSERT_SIZEOF  (combat_dodge_meta_anim,            0x50);
    ASSERT_OFFSETOF(combat_dodge_meta_anim, key_anims, 0x38);
    ASSERT_OFFSETOF(combat_dodge_meta_anim, unk_4c,    0x4C);

    ASSERT_SIZEOF  (combat_block_meta_anim,            0x50);
    ASSERT_OFFSETOF(combat_block_meta_anim, key_anims, 0x38);
    ASSERT_OFFSETOF(combat_block_meta_anim, unk_4c,    0x4C);
} // treyarch
