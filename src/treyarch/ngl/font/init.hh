#pragma once

#include "treyarch/base/tl_init_list.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl {
    void font_init();

    namespace references {
        inline util::memory_reference<tl_init_list> font_init_list { 0x00F522E0 };
    } // references
}} // treyarch::ngl
