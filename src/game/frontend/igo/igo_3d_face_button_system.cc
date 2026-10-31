#include <new>

#include "game/frontend/igo/igo_3d_face_button_system.hh"

using namespace treyarch;

// sub_68DCF0
string_hash igo_3d_face_button_system::get_button_widget_key(u32 kind) {
    if (kind <= 11)
        return references::button_widget_keys.get()[kind];

    string_hash result;

    result.initialize(mash::ALLOCATED, "");

    return result;
}

// sub_68DD30
mash::string* igo_3d_face_button_system::get_button_anim_name(mash::string* result, u32 kind, u32 index) {
    if (kind > 11 || index > 22)
        return new (result) mash::string("");

    return new (result) mash::string(references::button_anim_names.get()[kind][index].text);
}
