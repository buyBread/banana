#pragma once

#include <cassert>

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace chuck { namespace script_library {
    /* slc_manager::setup (sub_97AAB0) runs from game::load_this_level and registers every class and function (sub_83D730);
       slc_manager::destroy runs from game::unload_current_level.
       registration order is the BSL operand ABI, so nothing may be inserted before shipped entries. */
    class slc_manager : public singleton_instance<slc_manager, 0x01124C64> {

    public:
        dinkumware::vector<script_library_class*>* classes; // registration order; looked up by index (sub_A20F90)

        static void destroy();
    };

    ASSERT_SIZEOF(slc_manager, 0x04);
}}} // treyarch::chuck::script_library
