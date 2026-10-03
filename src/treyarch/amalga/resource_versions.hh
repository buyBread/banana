#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_versions {
        u32 pack_version;
        u32 entity_version;
        u32 nonentity_version;
        u32 raw_version;
        u32 auto_version;

        bool verify(string_hash filename) const;
    };

    namespace references {
        // the versions this executable was built against
        inline util::memory_reference<u32> resource_pack_version           { 0x00E76F30 };
        inline util::memory_reference<u32> resource_entity_mash_version    { 0x00E76F34 };
        inline util::memory_reference<u32> resource_nonentity_mash_version { 0x00E76F38 };
        inline util::memory_reference<u32> resource_raw_mash_version       { 0x00E76F3C };
    } // references

    ASSERT_SIZEOF  (resource_versions,                    0x14);
    ASSERT_OFFSETOF(resource_versions, entity_version,    0x04);
    ASSERT_OFFSETOF(resource_versions, nonentity_version, 0x08);
    ASSERT_OFFSETOF(resource_versions, raw_version,       0x0C);
    ASSERT_OFFSETOF(resource_versions, auto_version,      0x10);
}} // treyarch::amalga
