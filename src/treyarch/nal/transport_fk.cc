#include "treyarch/nal/transport_fk.hh"

using namespace treyarch;

// sub_9CD1F0
bool nal::transport_fk::get_bone_index(fixed_string* name, u32* index) {
    for (u32 i = 0; i < bone_count; i++) {
        if (bones[i].name.hash == name->hash) {
            *index = i;

            return true;
        }
    }

    *index = 0;

    return false;
}
