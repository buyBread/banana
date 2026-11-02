#pragma once

#include "game/wds/entity/conglomerate.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class door : public conglomerate {

    public:
        u8 reserved_198[0x04];

        explicit door(mash::generating_vtable_version so_do_nothing) : conglomerate(so_do_nothing) {}

        // sub_667E00
        ~door() override = default;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool is_a_door() const override;
        void post_unmash_fixup(amalga::resource_pack_slot* pack_slot) override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF(door, 0x19C);
} // treyarch
