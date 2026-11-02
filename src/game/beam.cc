#include <cmath>

#include "retail.hh"
#include "game/beam.hh"
#include "game/filespec.hh"
#include "game/fixed_string.hh"
#include "game/hash/algo.hh"
#include "game/math/references.hh"
#include "game/wds/entity/moved_entities.hh"
#include "treyarch/math/trig.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"

using namespace treyarch;

// sub_72C6E0
void beam::set_thickness(f32 value) {
    thickness = value;
}

// sub_72C700
void beam::set_max_length(f32 length) {
    max_length = length;

    if (max_length < 0.0f)
        max_length = 0.0f;
}

// sub_72C730
void beam::set_beam_color(const color32 &value) {
    my_start_color = value;
    my_end_color   = value;
}

// sub_7362B0
void beam::set_point_to_point(const vector3d &from, const vector3d &to) { // turns +z toward `to` and places the beam at `from`
    const vector3d delta(to.x - from.x, to.y - from.y, to.z - from.z);

    const f32 length = std::sqrt((f32)((f64)delta.x * delta.x + (f64)delta.y * delta.y + (f64)delta.z * delta.z));

    // set_max_length and set_start_dist, inlined
    max_length = length;

    if (max_length < 0.0f)
        max_length = 0.0f;

    start_dist = 0.0f;

    if (start_dist > max_length)
        start_dist = max_length;

    if (start_dist < 0.0f)
        start_dist = 0.0f;

    if (length > 0.0f) {
        if (delta.x != 0.0f || delta.y != 0.0f) {
            const f32      inverse = (f32)(1.0 / length);
            const vector3d direction(delta.x * inverse, delta.y * inverse, delta.z * inverse);

            po rotation;
            rotation.identity();

            const vector3d &z = references::unk_00e771e8.get();

            f32 dot = direction * z;

            math::vector4 axis(z.z * direction.y - z.y * direction.z,
                               z.x * direction.z - z.z * direction.x,
                               z.y * direction.x - z.x * direction.y,
                               0.0f);

            if (dot < -1.0f)
                dot = -1.0f;
            else if (dot > 1.0f)
                dot = 1.0f;

            f32 angle = (f32)(1.570796370506287 - math::asin(dot));

            rotation.set_rot(axis, angle);
            set_rel_po(rotation);
        } else if (delta.z > 0.0f) {
            // straight up keeps the position and drops the rotation
            vector3d position = my_rel_po->get_position();

            set_rel_po(*(const po*)&references::unk_01031d80.get());
            set_rel_position(position);
        } else if (delta.z < 0.0f) {
            // straight down is a half turn about y
            po rotation;
            rotation.identity();

            const vector3d &y = references::unk_00e771dc.get();

            math::vector4 axis(y.x, y.y, y.z, 0.0f);
            f32           half_turn = 3.14159265f;

            rotation.set_rot(axis, half_turn);
            rotation.set_position(my_rel_po->get_position());
            set_rel_po(rotation);
        }

        set_rel_position(from);
    }

    clear_flag(last_frame_valid);

    moved_entities::add_moved(my_handle);
}

// sub_736630
void beam::set_material(const mash::string &name) { // the texture named by the file name part of name
    if (!my_material) {
        // retail allocates 0x28 bytes for it
        void* memory = mm::allocate(0x28);

        my_material = memory ? (ngl::shaders::pcuv::pcuv_material*)retail::sub_8A8FB0((u32*)memory) : nullptr;
    } else if (my_material->texture_data)
        my_material->texture_data = nullptr;

    filespec spec(name);

    fixed_string texture_name;

    texture_name.hash = string_hash(hash::djb2(spec.name.c_str()));
    texture_name.set_text(spec.name.c_str());

    my_material->texture_data = (ngl::texture*)retail::sub_9E45A0(&texture_name.text);

    if (texture_name.text)
        mm::free(texture_name.text);
}
