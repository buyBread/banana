#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    // mashed ones get their retail vtables from the type registry
    class animation_blend : public mash::mash_virtual_base {

    public:
        u8          pad_004[0x04];
        nal_anim_id anim;
        f32         unk_28;
        f32         unk_2c;
    };

    class animation_blends_list : public mash::mash_virtual_base {

    public:
        f32 unk_04;
        f32 unk_08;

        mash::vector
            <animation_blend> blends;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (animation_blend,         0x30);
    ASSERT_OFFSETOF(animation_blend, anim,   0x08);
    ASSERT_OFFSETOF(animation_blend, unk_28, 0x28);
    ASSERT_OFFSETOF(animation_blend, unk_2c, 0x2C);

    ASSERT_SIZEOF  (animation_blends_list,         0x20);
    ASSERT_OFFSETOF(animation_blends_list, unk_04, 0x04);
    ASSERT_OFFSETOF(animation_blends_list, unk_08, 0x08);
    ASSERT_OFFSETOF(animation_blends_list, blends, 0x0C);
}} // treyarch::als
