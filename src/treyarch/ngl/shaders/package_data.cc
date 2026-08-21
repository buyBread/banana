#include <windows.h>

#include "banana/logging.hh"
#include "treyarch/ngl/shaders/package.hh"
#include "treyarch/ngl/shaders/package_data.hh"
#include "treyarch/ngl/shaders/resource_ids.hh"

using namespace treyarch;

extern "C" IMAGE_DOS_HEADER __ImageBase;

static const u8*                            data;
static       u32                            size;
static const ngl::shaders::package::header* header;
static const ngl::shaders::package::entry*  entries;
static       bool                           initialized;
static       bool                           valid;

static bool validate_package() {
    using namespace ngl::shaders;

    HMODULE module   = (HMODULE)&__ImageBase;
    HRSRC   resource = FindResourceA(module,
                                     MAKEINTRESOURCEA(NGL_SHADER_PACKAGE),
                                     RT_RCDATA);

    if (!resource)
        return false;

            size   = SizeofResource(module, resource);
    HGLOBAL handle = LoadResource(module, resource);
    
    data = handle ? (const u8*)LockResource(handle) : nullptr;

    if (!data || size < sizeof(package::header))
        return false;

    header = (const package::header*)data;

    if (header->magic              != package::magic          ||
        header->format_version     != package::format_version ||
        header->header_size        != sizeof(package::header) ||
        header->package_size       != size                    ||
        header->entry_table_offset != sizeof(package::header))
        
        return false;

    u64 entry_table_end = (u64)header->entry_table_offset + (u64)header->entry_count * sizeof(package::entry);

    if (entry_table_end > size || header->bytecode_offset < entry_table_end)
        return false;

    entries = (const package::entry*)(data + header->entry_table_offset);

    u32 previous_key = 0;

    for (u32 index = 0; index < header->entry_count; ++index) {
        const package::entry &entry = entries[index];
        u64 bytecode_end = (u64)entry.bytecode_offset + entry.bytecode_size;

        if ((index && entry.key   <= previous_key)           ||
            entry.bytecode_offset  < header->bytecode_offset ||
            bytecode_end           > size                    ||
            entry.bytecode_offset  % sizeof(u32)             ||
            entry.bytecode_size    % sizeof(u32)             ||
            !entry.bytecode_size)

            return false;

        previous_key = entry.key;
    }

    u32 expected_entry_count = 0;

    for (u16 value = 0; value < (u16)e_shader_program::count; ++value)
        expected_entry_count += (u32)get_permutation_count((e_shader_program)value);

    return header->entry_count == expected_entry_count;
}

static const ngl::shaders::package::entry* find_entry(u32 key) {
    u32 first = 0;
    u32 count = header->entry_count;

    while (count) {
        u32 step  = count / 2;
        u32 index = first + step;

        if (entries[index].key < key) {
            first = index + 1;
            count -= step + 1;
        } else
            count = step;
    }

    if (first >= header->entry_count || entries[first].key != key)
        return nullptr;

    return &entries[first];
}

std::span<const u8> ngl::shaders::package_data::load(shader_key key) {
    if (!initialized) {
        valid = validate_package();
        initialized = true;

        if (!valid)
            banana::log.err("embedded NGL shader package is invalid");
    }

    if (!valid)
        return {};

    const package::entry* entry = find_entry(encode_shader_key(key));

    if (!entry)
        return {};

    return { data + entry->bytecode_offset, entry->bytecode_size };
}
