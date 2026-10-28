#pragma once

#include "game/amalga/resource_types.hh"
#include "game/amalga/resource_versions.hh"
#include "treyarch/apk/file.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

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
        apk::data_reference*                              resource_references;
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
        u32                            unk_18;
        u8*                            adjusted_payload;
        resource_descriptor_extension* extension;
        u32                            unk_24;
        u32                            unk_28;

        // sub_735FF0
        u8* get_adjusted_payload() const {
            return adjusted_payload;
        }

        // sub_72DF90
        // inlined @ sub_73A130, sub_75F8D0, sub_75E2C0
        u8* calculate_adjusted_payload() const;

        // key is a resource_key*, entry a resource_descriptor**
        static i32 compare_name_hash(const void* key, const void* entry);

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
        apk::file*                         merged_apk_file;
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

        resource_descriptor* find_local_resource(const resource_key &key) const;
        resource_descriptor* find_resource(const resource_key &key, resource_directory** out_dir);

        bool get_resource_descriptor(const resource_key          &key,
                                           u32*                   size,
                                           resource_pack_slot**   slot,
                                           resource_descriptor**  descriptor);

        u8* get_resource(const resource_key &key, u32* size, resource_pack_slot** slot);

        void* find_local_merged_resource(string_hash name, u32 type) const;
        void* find_merged_resource(string_hash name, u32 type, resource_directory** out_dir);

        void* get_merged_resource(string_hash name, u32 type, resource_pack_slot** slot);
        void* get_merged_resource_or_default(string_hash name, u32 type, resource_pack_slot** slot); // a missing texture comes back as the white one
    };

    // resource_key::definitions, filled at startup; the arrays are per platform (ps3, pc, xenon)
    struct resource_type_definition_t {
        const char* singular;
        const char* plural;
        u32         category;          // 0 nonentity, 1 entity, 2 raw; also picks how adjusted_payload follows from raw_payload
        u32         version;           // the category's mash version
        u32         apk_type;          // the type's four_cc
        u8          unk_14;
        u8          unk_15;
        bool        held_in_apk;       // texture through efx
        u32         unk_18;
        u32         unk_1c;
        i32         alignment[3];
        const char* extension[3];      // ".PCTEXTURE"
        string_hash extension_hash[3]; // "PCTEXTURE"
        const char* directory[3];      // "PCTEXTURE\\"
    };

    namespace references {
        inline util::memory_reference<std::array<resource_type_definition_t, (size_t)e_resource_type::count>> resource_type_definitions { 0x00E77370 };
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

    ASSERT_SIZEOF  (resource_type_definition_t,                 0x50);
    ASSERT_OFFSETOF(resource_type_definition_t, category,       0x08);
    ASSERT_OFFSETOF(resource_type_definition_t, apk_type,       0x10);
    ASSERT_OFFSETOF(resource_type_definition_t, held_in_apk,    0x16);
    ASSERT_OFFSETOF(resource_type_definition_t, alignment,      0x20);
    ASSERT_OFFSETOF(resource_type_definition_t, extension,      0x2C);
    ASSERT_OFFSETOF(resource_type_definition_t, extension_hash, 0x38);
    ASSERT_OFFSETOF(resource_type_definition_t, directory,      0x44);
}} // treyarch::amalga
