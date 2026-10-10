#pragma once

#include "treyarch/amalga/resource_types.hh"
#include "treyarch/amalga/resource_versions.hh"
#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_pack_slot;

    // the first 0x20 bytes of a decompressed pack
    struct resource_pack_header {
        resource_versions versions;
        u32               directory_offset; // the nested mash image
        u32               directory_size;   // where the payloads may start, same as the toc's archive_header_size
        u32               unk_1c;
    };

    struct resource_descriptor_extension_entry {
        u32 unk_00[9];

        void construct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct resource_descriptor_extension {
        merged_apk::data_reference*                       resource_references;
        u8*                                               string_base;
        u32                                               unk_08;
        mash::vector<resource_descriptor_extension_entry> unk_0c;

        void construct_mashed_class() {
            unk_0c.construct_mashed_class();
        }

        // inlined @ sub_76B810
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_0c.unmash(mash_info, this, buffer);
        }
    };

    struct resource_descriptor {
        u32                            name_hash;
        e_resource_type                type;
        u32                            payload_offset;
        u32                            payload_size;
        u32                            secondary_size;
        u8*                            raw_payload;
        u32                            unknown_18;
        u8*                            adjusted_payload;
        resource_descriptor_extension* extension;
        u32                            unknown_24;
        u32                            unknown_28;

        // sub_735FF0
        u8* get_adjusted_payload() const {
            return adjusted_payload;
        }

        // inlined @ sub_73A130, sub_75F8D0
        u8* calculate_adjusted_payload() const;

        // inlined @ sub_76E390
        void construct_mashed_class() {
            if (extension)
                extension->construct_mashed_class();
        }

        // inlined @ sub_76B810
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer);
    };

    // a one-byte object; nothing reads it
    struct resource_directory_unk_3c4 {
        u8 unk_00;

        void construct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct resource_directory {
        u32                                handle;           // the directory's own address, set once it's read
        mash::vector<resource_descriptor>  descriptors;
        u32                                type_starts[(size_t)e_resource_type::count];
        u32                                type_counts[(size_t)e_resource_type::count];
        u8                                 type_state[(size_t)e_resource_type::count];
        merged_apk::file*                  merged_apk_file;
        resource_directory_unk_3c4*        unk_3c4;
        resource_pack_slot*                pack_slot;
        u32                                parent;           // the parent pack directory's handle
        u8                                 unk_3d0[8];

        // inlined @ sub_76ECF0
        void construct_mashed_class() {
            handle = 0;
            descriptors.construct_mashed_class();
        }

        void unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer);

        void constructor_common(resource_pack_slot* slot);
    };

    // filled at startup, one 0x50-byte record per resource type
    struct resource_type_record {
        u32 unk_00[2];
        u32 payload_mode; // how adjusted_payload follows from raw_payload: 0 adds its +4 word, 2 adds 8, others add nothing
        u32 unk_0c[17];
    };

    namespace references {
        inline util::memory_reference<std::array<resource_type_record, (size_t)e_resource_type::count>> resource_type_records { 0x00E77370 };
    } // references

    ASSERT_SIZEOF  (resource_pack_header,                   0x20);
    ASSERT_OFFSETOF(resource_pack_header, directory_offset, 0x14);
    ASSERT_OFFSETOF(resource_pack_header, directory_size,   0x18);

    ASSERT_SIZEOF  (resource_descriptor_extension_entry, 0x24);

    ASSERT_SIZEOF  (resource_descriptor_extension,                      0x20);
    ASSERT_OFFSETOF(resource_descriptor_extension, resource_references, 0x00);
    ASSERT_OFFSETOF(resource_descriptor_extension, string_base,         0x04);
    ASSERT_OFFSETOF(resource_descriptor_extension, unk_0c,              0x0C);

    ASSERT_SIZEOF  (resource_descriptor,                   0x2C);
    ASSERT_OFFSETOF(resource_descriptor, type,             0x04);
    ASSERT_OFFSETOF(resource_descriptor, payload_offset,   0x08);
    ASSERT_OFFSETOF(resource_descriptor, raw_payload,      0x14);
    ASSERT_OFFSETOF(resource_descriptor, adjusted_payload, 0x1C);
    ASSERT_OFFSETOF(resource_descriptor, extension,        0x20);

    ASSERT_SIZEOF  (resource_directory,                  0x3D8);
    ASSERT_OFFSETOF(resource_directory, descriptors,     0x04 );
    ASSERT_OFFSETOF(resource_directory, type_starts,     0x18 );
    ASSERT_OFFSETOF(resource_directory, type_counts,     0x1B8);
    ASSERT_OFFSETOF(resource_directory, type_state,      0x358);
    ASSERT_OFFSETOF(resource_directory, merged_apk_file, 0x3C0);
    ASSERT_OFFSETOF(resource_directory, unk_3c4,         0x3C4);
    ASSERT_OFFSETOF(resource_directory, pack_slot,       0x3C8);
    ASSERT_OFFSETOF(resource_directory, parent,          0x3CC);

    ASSERT_SIZEOF  (resource_type_record,               0x50);
    ASSERT_OFFSETOF(resource_type_record, payload_mode, 0x08);
}} // treyarch::amalga
