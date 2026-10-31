#include "retail.hh"
#include "game/math/algebra/quaternion.hh"

using namespace treyarch;

// sub_48E070
treyarch::quaternion math::slerp(const treyarch::quaternion &q0,
                                 const treyarch::quaternion &q1,
                                       f32                   t) { // the blend works on copies

    const quaternion b = q1;
    const quaternion a = q0;

    quaternion blended;
    retail::sub_48D240((f32*)&blended, 1.0f - t, t, (f32*)&a, (f32*)&b); // quaternion blend

    return treyarch::quaternion(blended.val());
}
