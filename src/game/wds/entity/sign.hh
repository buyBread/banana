#pragma once

#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class sign : public actor {

    public:
        u8 unk_0e8[0x0C]; // a mash::string the destructor finalizes

        explicit sign(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~sign() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool            is_a_sign() const override;
        e_entity_flavor get_flavor() const override;
        u32             get_entity_size() const override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (sign,          0xF4);
    ASSERT_OFFSETOF(sign, unk_0e8, 0xE8);
} // treyarch
