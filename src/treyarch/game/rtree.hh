#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct rtree;
    struct visitor;

    using visit_function = i32(*)(visitor*, i32);

    struct visitor_vtable {
        void*          destroy;
        visit_function visit;
    };

    struct visitor {
        visitor_vtable* vtable;
    };

    ASSERT_SIZEOF(visitor, 0x04);

    void query_sphere(      rtree*    tree,
                      const vector3d &center,
                            f32       radius,
                            visitor*  result_visitor);
} // treyarch
