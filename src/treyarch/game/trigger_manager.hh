#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class trigger_manager; // impl?

    namespace references {
        inline util::memory_reference<trigger_manager*> trigger_manager { 0x0102FE48 };
    } // references
} // treyarch
