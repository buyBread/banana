#include "treyarch/amalga/resource_amalgatoc.hh"

using namespace treyarch;

// sub_72DF70
i32 amalga::resource_amalgatoc_pack_entry::compare_name_hash(const void* key, const void* entry) {
    const u32 find_me = ((const string_hash*)key)->source_hash_code;
    const u32 hash    = (*(resource_amalgatoc_pack_entry* const*)entry)->name_hash.source_hash_code;

    if (find_me <= hash)
        return -(i32)(find_me < hash);

    return 1;
}
