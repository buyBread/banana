#include "treyarch/nal/skeleton_map.hh"

using namespace treyarch;

// sub_9CD510
nal::skeleton_map_entry* nal::skeleton_map::get_entry(fixed_string* source_name) {
    for (u32 i = 0; i < entry_count; i++) {
        if (entries[i]->source_name.hash == source_name->hash)
            return entries[i];
    }

    return nullptr;
}
