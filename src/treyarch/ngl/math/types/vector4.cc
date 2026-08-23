#include "treyarch/ngl/math/types/vector4.hh"
#include "treyarch/ngl/math/types/matrix4x4.hh"

using namespace treyarch::ngl;

vector4 vector4::transform_plane(const matrix4x4 &matrix) const {
        vector4 transformed(x * matrix[0][0] + y * matrix[1][0] + z * matrix[2][0],
                            x * matrix[0][1] + y * matrix[1][1] + z * matrix[2][1],
                            x * matrix[0][2] + y * matrix[1][2] + z * matrix[2][2],
                            0.0f);

        transformed.w = matrix[3][0] * transformed.x +
                        matrix[3][1] * transformed.y +
                        matrix[3][2] * transformed.z + w;

        return transformed;
    }