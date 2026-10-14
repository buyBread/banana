#include "retail.hh"
#include "treyarch/game/reference.hh"

using namespace treyarch;

// sub_7D3CE0
template<> void reference<ngl::texture>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (ngl::texture*)*retail::sub_7D3800(&found, (i32)name.source_hash_code); // universal_resource::get<texture>

    if (pointer)
        retail::sub_7B7720((u32*)this);
}

// inlined @ sub_7DA210
template<> void reference<meshmap>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (meshmap*)*retail::sub_7D38C0(&found, (i32)name.source_hash_code); // universal_resource::get<meshmap>
}

// inlined @ sub_7DA270
template<> void reference<adjustment_layers_library>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (adjustment_layers_library*)*retail::sub_7D38F0(&found, (i32)name.source_hash_code); // universal_resource::get<adjustment_layers_library>
}
