#pragma once

#include "treyarch/game/als/meta_anim.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/array_replacement.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    class als_meta_anim_swing : public als_meta_anim_base {

    public:
        mash::array_replacement
            <meta_key_anim, 8> keys;
        u32                    unk_2c;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        nal::anim*              get_anim_proxy() const override;
        nal_als_instance_bridge create_anim_inst(nal::transport_list*    the_skel,
                                                 u32                     flags,
                                                 nal::skeleton_map*      skel_map,
                                                 u32                     unk,
                                                 void*                   the_anim,
                                                 animation_logic_system* the_als,
                                                 state_machine*          the_layer) override;
        void                    load_key_anims() override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (als_meta_anim_swing,         0x30);
    ASSERT_OFFSETOF(als_meta_anim_swing, keys,   0x28);
    ASSERT_OFFSETOF(als_meta_anim_swing, unk_2c, 0x2C);
}} // treyarch::als
