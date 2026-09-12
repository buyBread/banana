#pragma once

#include <cstddef>

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class event : public mash::mash_virtual_base {

        string_hash event_type_id;
        u32         raised_frame;
        bool        autokill;
        bool        from_mash;
        u8          padding_0e[2];

    public:
        explicit event(string_hash event_type_id, bool autokill = false);

        void* operator new(std::size_t size);
        void  operator delete(void* allocation) noexcept;

        void construct_mashed_class() override;
        mash::virtual_types_key get_virtual_type_key() const override;
        void copy_values(const mash::mash_virtual_base*) override {}
        bool is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32  get_mash_sizeof() const override { return sizeof(*this); }

        virtual u32  get_raised_frame() const { return raised_frame; }
        virtual void raise();
        virtual bool is_an_event() const { return true; }

        string_hash type_id() const noexcept { return event_type_id; }
    };

    ASSERT_SIZEOF(event, 0x10);
} // treyarch
