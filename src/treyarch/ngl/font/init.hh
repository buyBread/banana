#pragma once

#include "treyarch/ngl/init_list/init_list.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    void font_init();

    namespace references {
        inline ::util::memory_reference<init_list> font_init_list { 0x00F522E0 };
    } // references
}} // treyarch::ngl
