#include "treyarch/shared/math/types/vector4.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"

using namespace treyarch;

vector4 vector4::transform_plane(const matrix4x4 &matrix) const {
    vector4 transformed((f32)((f64)x * matrix[0][0] + (f64)y * matrix[1][0] + (f64)z * matrix[2][0]),
                        (f32)((f64)x * matrix[0][1] + (f64)y * matrix[1][1] + (f64)z * matrix[2][1]),
                        (f32)((f64)x * matrix[0][2] + (f64)y * matrix[1][2] + (f64)z * matrix[2][2]),
                        0.0f);

    transformed.w = (f32)((f64)matrix[3][0] * transformed.x +
                          (f64)matrix[3][1] * transformed.y +
                          (f64)matrix[3][2] * transformed.z + w);

    return transformed;
}
