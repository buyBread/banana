#pragma once

#include "treyarch/ngl/init_list/init_list.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace fx {
    void init();

    namespace references {
        inline util::memory_reference<init_list_function> init_list { 0x00F52A80 };
        inline util::memory_reference<u32> initialization_state { 0x01117190 };
    } // references
}}} // treyarch::ngl::fx
