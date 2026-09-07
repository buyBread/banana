#pragma once

#include "treyarch/ngl/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace math {
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

    void query_sphere(      rtree*   tree,
                      const vector3 &center,
                            f32      radius,
                            visitor* result_visitor);
}}} // treyarch::ngl::math
