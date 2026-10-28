#include "game/reference.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

// sub_7D3CE0
void reference<ngl::texture>::initialize(mash::allocation_scope) {
    string_hash empty;

    empty.initialize(mash::ALLOCATED);

    if (name == empty) {
        pointer = nullptr;

        return;
    }

    pointer = universal_resource::get<ngl::texture>(name);

    if (pointer)
        calculate_size_class();
}

// sub_7B7720
void reference<ngl::texture>::calculate_size_class() {
    const i8 width_class  = dimension_class(pointer->gpu_texture.width);
    const i8 height_class = dimension_class(pointer->gpu_texture.height);

    size_class = (height_class + 12) * 13 - width_class * 12;
}

// inlined @ sub_7B7720
i8 reference<ngl::texture>::dimension_class(u32 dimension) { // log2 of a power of two up to 0x1000, else 0
    switch (dimension) {
        case 0x0002: return 1;
        case 0x0004: return 2;
        case 0x0008: return 3;
        case 0x0010: return 4;
        case 0x0020: return 5;
        case 0x0040: return 6;
        case 0x0080: return 7;
        case 0x0100: return 8;
        case 0x0200: return 9;
        case 0x0400: return 10;
        case 0x0800: return 11;
        case 0x1000: return 12;
        default:     return 0;
    }
}
