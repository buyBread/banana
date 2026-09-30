#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace references {
        // development_option<bool> RESTART_SKIP_THE_TITLE
        inline util::memory_reference<u8> restart_skip_the_title { 0x010300D4 };
    } // references
} // treyarch
