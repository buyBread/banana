#pragma once

#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/amalga/resource_versions.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    enum e_resource_amalgatoc_pack_entry_flags : u32 {
        resource_amalgatoc_pack_entry_flag_compressed  = 0x01, // read stored_size and decode NCH chunks, otherwise copy data_size raw bytes
        resource_amalgatoc_pack_entry_flag_shared_file = 0x02  // read from amalgapak_id instead of a loose pack file
    };

    struct resource_amalgatoc_pack_entry {
        string_hash                           name_hash;
        mash::string                          name;
        string_hash                           group_hash;          // parent pack, resolved when the directory is finalized
        u32                                   kind;
        u32                                   data_size[buffer_location_count];
        u32                                   stored_size[buffer_location_count];
        u32                                   unk_28;
        u32                                   unk_2c;              // -1 whenever there's no vram part
        u32                                   unk_30;
        u32                                   source_offset;
        u32                                   archive_header_size;
        u32                                   stored_size_total;   // the loose file's length
        u32                                   unk_40;              // copied into a slot that starts loading the pack
        e_resource_amalgatoc_pack_entry_flags flags;

        // key is a string_hash*, entry a resource_amalgatoc_pack_entry**
        static i32 compare_name_hash(const void* key, const void* entry);

        // inlined @ sub_7624E0
        void construct_mashed_class() {
            name.construct_mashed_class();
        }

        // inlined @ sub_75CBE0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name.unmash(mash_info, this, buffer);
        }
    };

    struct resource_amalgatoc {
        resource_versions                           versions;
        u32                                         flags;           // sm3's pack_data_offset; bit 0 makes streaming blocking
        mash::string                                name;
        mash::vector<resource_amalgatoc_pack_entry> pack_entries;    // sorted by name_hash
        mash::vector<resource_memory_map>           memory_maps;
        i32                                         memory_map_to_use;
        u32                                         resource_buffer_size[buffer_location_count];

        // inlined @ sub_76E400
        void construct_mashed_class() {
            name        .construct_mashed_class();
            pack_entries.construct_mashed_class();
            memory_maps .construct_mashed_class();
        }

        // inlined @ sub_76E310
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name        .unmash(mash_info, this, buffer);
            pack_entries.unmash(mash_info, this, buffer);
            memory_maps .unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF  (resource_amalgatoc_pack_entry,                      0x48);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, name,                0x04);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, group_hash,          0x10);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, kind,                0x14);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, data_size,           0x18);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, stored_size,         0x20);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, unk_28,              0x28);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, source_offset,       0x34);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, archive_header_size, 0x38);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, stored_size_total,   0x3C);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, unk_40,              0x40);
    ASSERT_OFFSETOF(resource_amalgatoc_pack_entry, flags,               0x44);

    ASSERT_SIZEOF  (resource_amalgatoc,                       0x58);
    ASSERT_OFFSETOF(resource_amalgatoc, flags,                0x14);
    ASSERT_OFFSETOF(resource_amalgatoc, name,                 0x18);
    ASSERT_OFFSETOF(resource_amalgatoc, pack_entries,         0x24);
    ASSERT_OFFSETOF(resource_amalgatoc, memory_maps,          0x38);
    ASSERT_OFFSETOF(resource_amalgatoc, memory_map_to_use,    0x4C);
    ASSERT_OFFSETOF(resource_amalgatoc, resource_buffer_size, 0x50);
}} // treyarch::amalga
