#pragma once

#include "treyarch/shared/boolx.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every nugget; mashed ones get their retail vtable from the type registry
    class nugget : public mash::mash_virtual_base {

    public:
        boolx from_mash;
        u8    pad_005[0x03];

        virtual void set_user_data(void* user_data) = 0;
        virtual void unk_028() = 0; // SM3's read_data
        virtual void unk_02c() = 0; // SM3's get_write_data
        virtual bool is_from_mash() const = 0;
    };

    ASSERT_SIZEOF  (nugget,            0x08);
    ASSERT_OFFSETOF(nugget, from_mash, 0x04);
} // treyarch
