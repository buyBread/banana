#include "game/math/po.hh"

using namespace treyarch;

// sub_402C60
po::po(const math::vector4 &desired_forward, const math::vector4 &normal, const math::vector4 &pos) {
    const math::vector4 position = pos;

    set_po(desired_forward, normal, position);
}

// sub_4015B0
void po::identity() {
    m.x = vector4d(1.0f, 0.0f, 0.0f, 0.0f);
    m.y = vector4d(0.0f, 1.0f, 0.0f, 0.0f);
    m.z = vector4d(0.0f, 0.0f, 1.0f, 0.0f);
    m.w = vector4d(0.0f, 0.0f, 0.0f, 1.0f);
}

// sub_424900
// inlined @ sub_967860
// inlined @ sub_626CA0
// inlined @ sub_626E30
void po::set_position(const vector3d &v) {
    m.w = vector4d(v.x, v.y, v.z, 1.0f);
}

// sub_686C30
void po::set_translate(const math::vector4 &translation) {
    m.x = vector4d(1.0f, 0.0f, 0.0f, 0.0f);
    m.y = vector4d(0.0f, 1.0f, 0.0f, 0.0f);
    m.z = vector4d(0.0f, 0.0f, 1.0f, 0.0f);
    m.w = vector4d(translation[0], translation[1], translation[2], 1.0f);
}

// sub_48E170
void po::set_scale(const math::vector4 &scale) {
    m.set_m33(math::mat33(math::dir3(scale[0], 0.0f,     0.0f),
                          math::dir3(0.0f,     scale[1], 0.0f),
                          math::dir3(0.0f,     0.0f,     scale[2])));

    m.w = vector4d(0.0f, 0.0f, 0.0f, 1.0f);
}

// sub_40A750
void po::set_rot(const math::vector4 &rot_dir, const f32 &rads) {
    m.set_m33(math::axis_angle_to_rot_mat(math::unitize(math::dir3(rot_dir)), -0.0f - rads));

    m.w = vector4d(0.0f, 0.0f, 0.0f, 1.0f);
}

// sub_5B74E0
void po::set_po(const vector3d &p, const quaternion &r, f32 s) {
    m.set_m33(math::rot_mat33(math::quat_mat33(r)));

    for (i32 i = 0; i < 3; i++)
        for (i32 j = 0; j < 4; j++)
            m[i][j] *= s;

    m.w = vector4d(p.x, p.y, p.z, 1.0f);
}

// sub_5FC820
void po::set_po(const vector3d &xdest, const vector3d &ydest, const vector3d &zdest, const vector3d &pos) {
    m.x = vector4d(xdest.x, xdest.y, xdest.z, 0.0f);
    m.y = vector4d(ydest.x, ydest.y, ydest.z, 0.0f);
    m.z = vector4d(zdest.x, zdest.y, zdest.z, 0.0f);
    m.w = vector4d(pos.x,   pos.y,   pos.z,   1.0f);
}

// sub_424960
void po::set_po(const vector3d &desired_forward, const vector3d &normal, const vector3d &pos) {
    const math::vector4 normal_4(normal.x, normal.y, normal.z, 0.0f);
    const math::unit_dir3 u = math::unitize(math::dir3(normal_4));

    const math::vector4 forward_4(desired_forward.x, desired_forward.y, desired_forward.z, 0.0f);
    const math::unit_dir3 f = math::unitize(math::dir3(forward_4));

    const math::unit_dir3 s = math::unitize(math::dir3((f32)((f64)u[1] * f[2] - (f64)u[2] * f[1]),
                                                       (f32)((f64)u[2] * f[0] - (f64)u[0] * f[2]),
                                                       (f32)((f64)u[0] * f[1] - (f64)u[1] * f[0])));

    m.x = vector4d(s[0], s[1], s[2], 0.0f);
    m.y = vector4d(u[0], u[1], u[2], 0.0f);
    m.z = vector4d((f32)((f64)s[1] * u[2] - (f64)s[2] * u[1]),
                   (f32)((f64)s[2] * u[0] - (f64)s[0] * u[2]),
                   (f32)((f64)s[0] * u[1] - (f64)s[1] * u[0]),
                   0.0f);
    m.w = vector4d(pos.x, pos.y, pos.z, 1.0f);
}

// sub_402260
void po::set_po(const math::vector4 &desired_forward, const math::vector4 &normal, const math::vector4 &pos) {
    const math::unit_dir3 u = math::unitize(math::dir3(normal));
    const math::unit_dir3 f = math::unitize(math::dir3(desired_forward));

    const math::unit_dir3 s = math::unitize(math::dir3((f32)((f64)u[1] * f[2] - (f64)u[2] * f[1]),
                                                       (f32)((f64)u[2] * f[0] - (f64)u[0] * f[2]),
                                                       (f32)((f64)u[0] * f[1] - (f64)u[1] * f[0])));

    m.x = vector4d(s[0], s[1], s[2], 0.0f);
    m.y = vector4d(u[0], u[1], u[2], 0.0f);
    m.z = vector4d((f32)((f64)s[1] * u[2] - (f64)s[2] * u[1]),
                   (f32)((f64)s[2] * u[0] - (f64)s[0] * u[2]),
                   (f32)((f64)s[0] * u[1] - (f64)s[1] * u[0]),
                   0.0f);
    m.w = vector4d(pos[0], pos[1], pos[2], 1.0f);
}

// sub_44DE20
void po::set_facing(const math::vector4 &point) { // z at the point, x level; straight up or down takes the fixed axes
    const math::unit_dir3 n = math::unitize(math::dir3(point[0] - m.w.x,
                                                       point[1] - m.w.y,
                                                       point[2] - m.w.z));

    if (0.9999f > std::fabs(n[1])) {
        const math::vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

        const math::unit_dir3 r = math::unitize(math::dir3((f32)((f64)up[1] * n[2] - (f64)up[2] * n[1]),
                                                           (f32)((f64)up[2] * n[0] - (f64)up[0] * n[2]),
                                                           (f32)((f64)up[0] * n[1] - (f64)up[1] * n[0])));

        m.x = vector4d(r[0], r[1], r[2], 0.0f);
        m.y = vector4d((f32)((f64)n[1] * r[2] - (f64)n[2] * r[1]),
                       (f32)((f64)n[2] * r[0] - (f64)n[0] * r[2]),
                       (f32)((f64)n[0] * r[1] - (f64)n[1] * r[0]),
                       0.0f);
        m.z = vector4d(n[0], n[1], n[2], 0.0f);

        return;
    }

    m.x = vector4d(1.0f, 0.0f, 0.0f, 0.0f);

    if (n[1] > 0.0f) {
        m.y = vector4d(0.0f, 0.0f, -1.0f, 0.0f);
        m.z = vector4d(0.0f, 1.0f,  0.0f, 0.0f);
    } else {
        m.y = vector4d(0.0f,  0.0f, 1.0f, 0.0f);
        m.z = vector4d(0.0f, -1.0f, 0.0f, 0.0f);
    }
}

// sub_44E1E0
void po::set_facing(vector3d point) {
    set_facing(math::vector4(point.x, point.y, point.z, 0.0f));
}

// sub_9595B0
void po::fixup() { // x kept, then z and y rebuilt square to it
    const math::unit_dir3  newx = math::unitize(reinterpret_cast<const math::dir3&>(m.x));
    const vector4d        &y    = m.y;

    const math::unit_dir3 newz = math::unitize(math::dir3((f32)((f64)newx[1] * y.z - (f64)newx[2] * y.y),
                                                          (f32)((f64)newx[2] * y.x - (f64)newx[0] * y.z),
                                                          (f32)((f64)newx[0] * y.y - (f64)newx[1] * y.x)));

    m.x = vector4d(newx[0], newx[1], newx[2], 0.0f);
    m.y = vector4d((f32)((f64)newz[1] * newx[2] - (f64)newz[2] * newx[1]),
                   (f32)((f64)newz[2] * newx[0] - (f64)newz[0] * newx[2]),
                   (f32)((f64)newz[0] * newx[1] - (f64)newz[1] * newx[0]),
                   0.0f);
    m.z = vector4d(newz[0], newz[1], newz[2], 0.0f);
}

// sub_48E2C0
// inlined @ sub_4B77A0
po po::inverse() const { // the rotation turned back and the translation undone
    matrix4x4 transposed;

    transposed.x.x = m.x.x;
    transposed.x.y = m.y.x;
    transposed.x.z = m.z.x;
    transposed.y.x = m.x.y;
    transposed.y.y = m.y.y;
    transposed.y.z = m.z.y;
    transposed.z.x = m.x.z;
    transposed.z.y = m.y.z;
    transposed.z.z = m.z.z;
    transposed.w.x = -0.0f - (f32)((f64)m.w.x * m.x.x + (f64)m.w.y * m.x.y + (f64)m.w.z * m.x.z);
    transposed.w.y = -0.0f - (f32)((f64)m.w.x * m.y.x + (f64)m.w.y * m.y.y + (f64)m.w.z * m.y.z);
    transposed.w.z = -0.0f - (f32)((f64)m.w.x * m.z.x + (f64)m.w.y * m.z.y + (f64)m.w.z * m.z.z);

    po result;
    reinterpret_cast<math::mat44&>(result) = math::mat44(reinterpret_cast<const math::mat43&>(transposed));

    return result;
}

// sub_44E230
void treyarch::fast_po_mul(po &res, const po &a, const po &b) { // a first, then b
    reinterpret_cast<math::mat44&>(res) = math::mat44(reinterpret_cast<const math::mat43&>(a) *
                                                      reinterpret_cast<const math::mat43&>(b));
}
