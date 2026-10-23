#pragma once

#include "vc8/vector.hh"
#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace chuck { namespace script_library {
    /* slc_manager::setup runs from game::load_this_level and registers every class and function;
       slc_manager::destroy runs from game::unload_current_level.
       registration order is the BSL operand ABI, so nothing may be inserted before shipped entries. */
    class slc_manager : public singleton_instance<slc_manager, 0x01124C64> {

    public:
        vc8::vector<script_library_class*>* classes; // registration order

        slc_manager();
       ~slc_manager();

        void add(script_library_class* slc);

        script_library_class* get_class(i32 index) const;

        static void setup();
        static void destroy();

    private:
        void init();
    };

    ASSERT_SIZEOF(slc_manager, 0x04);
}}} // treyarch::chuck::script_library
