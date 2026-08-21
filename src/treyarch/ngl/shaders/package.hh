#pragma once

#include <cstddef>

#include "description.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace package {
    inline constexpr u32 magic             = 0x534C474E;
    inline constexpr u16 format_version    = 1;
    inline constexpr u32 compiler_revision = 1;

#pragma pack(push, 1)
    struct header {
        u32 magic;
        u16 format_version;
        u16 header_size;
        u32 package_size;
        u32 entry_count;
        u32 entry_table_offset;
        u32 bytecode_offset;
    };

    struct entry {
        u32 key;
        u64 input_hash;
        u32 bytecode_offset;
        u32 bytecode_size;
    };
#pragma pack(pop)

    static_assert(sizeof(header) == 0x18);
    static_assert(sizeof(entry)  == 0x14);

    inline constexpr u64 hash_offset = 14695981039346656037ull;
    inline constexpr u64 hash_prime  = 1099511628211ull;

    inline u64 hash_bytes(u64 hash, const void* data, size_t size) {
        const u8* bytes = (const u8*)data;

        for (size_t index = 0; index < size; ++index) {
            hash ^= bytes[index];
            hash *= hash_prime;
        }

        return hash;
    }

    inline u64 hash_string(u64 hash, const char* value) {
        while (*value) {
            hash ^= (u8)*value++;
            hash *= hash_prime;
        }

        hash ^= 0;
        hash *= hash_prime;

        return hash;
    }

    inline u64 hash_input(const shader_description &description,
                          const void*               source,
                                size_t              source_size) {

        u64 hash = hash_offset;
        u32 key  = encode_shader_key(description.key);

        hash = hash_bytes (hash, &compiler_revision, sizeof(compiler_revision));
        hash = hash_bytes (hash, &key, sizeof(key));
        hash = hash_string(hash, description.source_path);
        hash = hash_string(hash, description.source_name);
        hash = hash_string(hash, description.entry_point);
        hash = hash_string(hash, description.profile);

        for (size_t index = 0; index < description.definition_count; ++index) {
            hash = hash_string(hash, description.definitions[index].name);
            hash = hash_string(hash, description.definitions[index].value);
        }

        return hash_bytes(hash, source, source_size);
    }
}}}} // treyarch::ngl::shaders::package
