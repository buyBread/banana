#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct resource_key {
        string_hash hash;
        u32         type;
    };

    ASSERT_SIZEOF  (resource_key,       0x08);
    ASSERT_OFFSETOF(resource_key, hash, 0x00);
    ASSERT_OFFSETOF(resource_key, type, 0x04);
} // treyarch
