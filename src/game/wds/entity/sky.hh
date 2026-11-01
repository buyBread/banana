#pragma once

#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class sky : public actor {

    public:
        explicit sky(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~sky() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

        bool            is_a_sky() const override;
        e_entity_flavor get_flavor() const override;

        static string_hash my_type_key;

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF(sky, 0xE8);
} // treyarch
