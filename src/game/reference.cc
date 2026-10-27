#include "retail.hh"
#include "game/reference.hh"

using namespace treyarch;

// sub_7D3CE0
void reference<ngl::texture>::initialize(mash::allocation_scope) {
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

// inlined @ sub_7DA0F0
template<> void reference<day_sky_color>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (day_sky_color*)*retail::sub_7D3950(&found, (i32)name.source_hash_code); // universal_resource::get<day_sky_color>
}

// inlined @ sub_7DA2D0
template<> void reference<environment>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (environment*)*retail::sub_7D3860(&found, (i32)name.source_hash_code); // universal_resource::get<environment>
}

// inlined @ sub_7A08A0
template<> void reference<light_behavior>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (light_behavior*)*retail::sub_794B20(&found, (i32)name.source_hash_code); // universal_resource::get<light_behavior>
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

// inlined @ sub_7DA090
template<> void reference<ngl::mesh>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (ngl::mesh*)*retail::sub_7D3830(&found, (i32)name.source_hash_code); // universal_resource::get<mesh>
}

// inlined @ sub_7DA150
template<> void reference<night_sky_color>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (night_sky_color*)*retail::sub_7D3980(&found, (i32)name.source_hash_code); // universal_resource::get<night_sky_color>
}

// inlined @ sub_7DA1B0
template<> void reference<outdoors>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (outdoors*)*retail::sub_7D3920(&found, (i32)name.source_hash_code); // universal_resource::get<outdoors>
}

// inlined @ sub_7F9B30
template<> void reference<quest_descriptions>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (quest_descriptions*)*retail::sub_7F2C80(&found, (i32)name.source_hash_code); // universal_resource::get<quest_descriptions>
}

// inlined @ sub_933270
template<> void reference<spawn_item_collection>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (spawn_item_collection*)*retail::sub_9225B0(&found, (i32)name.source_hash_code); // universal_resource::get<spawn_item_collection>
}

// inlined @ sub_9332D0
template<> void reference<spawn_point_list_collection>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (spawn_point_list_collection*)*retail::sub_9225E0(&found, (i32)name.source_hash_code); // universal_resource::get<spawn_point_list_collection>
}

// inlined @ sub_933330
template<> void reference<static_spawn_item_collection>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (static_spawn_item_collection*)*retail::sub_922610(&found, (i32)name.source_hash_code); // universal_resource::get<static_spawn_item_collection>
}

// inlined @ sub_7A1700
template<> void reference<tilemap>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    i32 found;

    pointer = (tilemap*)*retail::sub_794AF0(&found, (i32)name.source_hash_code); // universal_resource::get<tilemap>
}
