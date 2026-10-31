#include "game/math/algebra/quaternion.hh"

using namespace treyarch;

// sub_48E070
treyarch::quaternion math::slerp(const treyarch::quaternion &q0,
                                 const treyarch::quaternion &q1,
                                       f32                   t) { // the blend works on x-first copies

    const unit_quaternion b = q1;
    const unit_quaternion a = q0;

    return slerp(1.0f - t, t, a, b);
}
