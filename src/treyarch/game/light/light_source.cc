#include "retail.hh"
#include "treyarch/game/light/light_source.hh"

using namespace treyarch;

light_properties::light_properties(light_flavor flavor,
                                   color        diffuse_color,
                                   color        additive_color,
                                   f32          near_range,
                                   f32          cutoff_range) {

    using constructor = light_properties* (__thiscall*)(light_properties*, light_flavor, color, color, f32, f32);

    ((constructor)retail::sub_77DE40)(this, flavor, diffuse_color, additive_color, near_range, cutoff_range);
}

light_source::light_source(const light_properties &properties, const po* the_po, const string_hash &id) {
    retail::sub_78C780((i32*)this, (i32)&properties, (i32*)the_po, (i32*)&id);
}
