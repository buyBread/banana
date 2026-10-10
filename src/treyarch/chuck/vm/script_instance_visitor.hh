#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_instance;
    class script_object;

    enum e_visit_result : i32 {
        visit_result_continue,
        visit_result_skip_object,
        visit_result_halt_walk
    };

    // script_executable::walk_instances asks allow_object first, then visits that object's instances
    class script_instance_visitor {

    public:
        virtual ~script_instance_visitor() = default;

        virtual bool allow_object(const script_object*) {
            return true;
        }

        virtual e_visit_result visit(const script_instance*) {
            return visit_result_halt_walk;
        }
    };

    ASSERT_SIZEOF(script_instance_visitor, 0x04);
}}} // treyarch::chuck::vm
