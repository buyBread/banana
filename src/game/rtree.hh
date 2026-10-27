#pragma once

#include "game/math/algebra.hh"
#include "game/wds/subdivision_visitor.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct rtree;

    void query_sphere(      rtree*               tree,
                      const vector3d            &center,
                            f32                  radius,
                            subdivision_visitor* result_visitor);
} // treyarch
