#include <cstring>

#include "retail.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/math/quaternion.hh"
#include "treyarch/math/vector.hh"
#include "treyarch/base/memory.hh"
#include "treyarch/base/tl_defs.hh"

using namespace treyarch;

// sub_9CCCC0
nal::po nal::po_multiply(const po &a, const po &b) {
    math::rot_mat33 scratch;

    auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&b.get_m33().get_quaternion());

    const math::unit_dir3  &x  = rows->get_x();
    const math::unit_dir3  &y  = rows->get_y();
    const math::unit_dir3  &z  = rows->get_z();
    const math::vector4    &ta = *(const math::vector4*)&a.get_w();
    const math::vector4    &tb = *(const math::vector4*)&b.get_w();
    const math::quaternion &qa = *(const math::quaternion*)&a.get_m33().get_quaternion();
    const math::quaternion &qb = *(const math::quaternion*)&b.get_m33().get_quaternion();

    po result;

    *(math::vector4*)&result.get_w() = math::vector4((f32)((f64)ta[0] * x[0] + (f64)ta[1] * y[0] + (f64)ta[2] * z[0]) + tb[0],
                                                     (f32)((f64)ta[0] * x[1] + (f64)ta[1] * y[1] + (f64)ta[2] * z[1]) + tb[1],
                                                     (f32)((f64)ta[0] * x[2] + (f64)ta[1] * y[2] + (f64)ta[2] * z[2]) + tb[2],
                                                     ta[3] + tb[3]);

    *(math::quaternion*)&result.get_m33().get_quaternion() = qa * qb;

    return result;
}

// sub_9CCF80
nal::po nal::po_inverse(const po &value) {
    const math::quaternion &q = *(const math::quaternion*)&value.get_m33().get_quaternion();
    const math::vector4    &t = *(const math::vector4*)&value.get_w();

    f32 norm = (f32)((f64)q[0] * q[0] + (f64)q[1] * q[1] + (f64)q[2] * q[2] + (f64)q[3] * q[3]);

    po result;

    *(math::quaternion*)&result.get_m33().get_quaternion() = math::quaternion((~q).val() / norm);

    math::vector4   negated = -t;
    math::rot_mat33 scratch;

    auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&result.get_m33().get_quaternion());

    const math::unit_dir3 &x = rows->get_x();
    const math::unit_dir3 &y = rows->get_y();
    const math::unit_dir3 &z = rows->get_z();

    *(math::vector4*)&result.get_w() = math::vector4((f32)((f64)negated[0] * x[0] + (f64)negated[1] * y[0] + (f64)negated[2] * z[0]),
                                                     (f32)((f64)negated[0] * x[1] + (f64)negated[1] * y[1] + (f64)negated[2] * z[1]),
                                                     (f32)((f64)negated[0] * x[2] + (f64)negated[1] * y[2] + (f64)negated[2] * z[2]),
                                                     negated[3]);

    return result;
}

// sub_9CEB30
nal::pose::pose(nal::pose_layout* layout, void* preallocated_memory) {
    init(layout, preallocated_memory);
}

// sub_9CD1D0
nal::pose::~pose() {
    if (alloced_data)
        memory::free(pose_data);
}

// sub_9CEAB0
void nal::pose::init(nal::pose_layout* layout, void* preallocated_memory) {
    pose_layout  = layout;
    pose_data    = nullptr;
    alloced_data = 0;

    present.set_all_zeroes();

    if (preallocated_memory) {
        pose_data    = (u8*)preallocated_memory;
        alloced_data = 0;
    } else {
        pose_data    = (u8*)memory::allocate(tl_align(pose_layout->pose_size, 16), 16, 0);
        alloced_data = 1;
    }

    std::memset(pose_data, 0, pose_layout->pose_size);
}

// sub_9CEB80
void nal::pose::copy(const pose &pose) {
    std::memcpy(pose_data, pose.pose_data, pose_layout->pose_size);

    present = pose.present;
}

// inlined @ sub_9CD5B0
// inlined @ sub_9CDB30
// inlined @ sub_9CDE70
// inlined @ sub_9D06E0
// inlined @ sub_9CFDB0
// inlined @ sub_9CFF00
// inlined @ sub_9CFF70
// inlined @ sub_9D0640
// inlined @ sub_9D0780
// inlined @ sub_9D0440
// inlined @ sub_9D2030
// inlined @ sub_5F74C0
bool nal::pose::is_field_present(u32 field_type, i32 n) {
    if (!pose_layout->field_repeat_counts[field_type])
        return false;

    return present.is_one(pose_layout->field_bit_offsets[field_type] + n) != 0;
}

// inlined @ sub_9CF290
// inlined @ sub_9D1860
// inlined @ sub_9CECB0
// inlined @ sub_9D2030
// inlined @ sub_5F74C0
f32* nal::pose::get_field_data_ptr(u32 field_type, i32 n) {
    return (f32*)(pose_data + pose_layout->field_offsets[field_type]) + values_per_field[field_type] * n;
}
