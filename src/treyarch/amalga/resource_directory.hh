#pragma once

#include "treyarch/amalga/resource_types.hh"
#include "treyarch/amalga/apkf/file.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_descriptor_extension {
        apkf::data_reference* resource_references;
        u8*                   string_base;
        u8                    remaining[0x18];
    };

    struct resource_descriptor {
        u32                            name_hash;
        e_resource_type                type;
        u32                            payload_offset;
        u32                            payload_size;
        u32                            secondary_size;
        void*                          raw_payload;
        u32                            unknown_18;
        void*                          adjusted_payload;
        resource_descriptor_extension* extension;
        u32                            unknown_24;
        u32                            unknown_28;
    };

    struct resource_directory {
        u32                   unknown_00;
        u32                   unknown_04;
        u32                   resource_count;
        resource_descriptor** descriptors;
        u32                   fitted_count;
        u32                   unknown_14;
        u32                   type_starts[(size_t)e_resource_type::count];
        u32                   type_counts[(size_t)e_resource_type::count];
        u8                    type_state[(size_t)e_resource_type::count];
        apkf::file*           merged_apk_file;
        u8                    remaining[0x14];
    };

    struct resource_pack_slot {
        u8                  unknown_00[0x28];
        void*               reference_context;
        resource_directory* directory;
    };

    ASSERT_SIZEOF  (resource_descriptor_extension,                      0x20);
    ASSERT_OFFSETOF(resource_descriptor_extension, resource_references, 0x00);
    ASSERT_OFFSETOF(resource_descriptor_extension, string_base,         0x04);

    ASSERT_SIZEOF  (resource_descriptor,                   0x2C);
    ASSERT_OFFSETOF(resource_descriptor, type,             0x04);
    ASSERT_OFFSETOF(resource_descriptor, raw_payload,      0x14);
    ASSERT_OFFSETOF(resource_descriptor, adjusted_payload, 0x1C);
    ASSERT_OFFSETOF(resource_descriptor, extension,        0x20);

    ASSERT_SIZEOF  (resource_directory,                  0x3D8);
    ASSERT_OFFSETOF(resource_directory, descriptors,     0x0C );
    ASSERT_OFFSETOF(resource_directory, type_starts,     0x18 );
    ASSERT_OFFSETOF(resource_directory, type_counts,     0x1B8);
    ASSERT_OFFSETOF(resource_directory, type_state,      0x358);
    ASSERT_OFFSETOF(resource_directory, merged_apk_file, 0x3C0);

    ASSERT_OFFSETOF(resource_pack_slot, reference_context, 0x28);
    ASSERT_OFFSETOF(resource_pack_slot, directory,         0x2C);
}} // treyarch::amalga
