#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class zombie_manager; // impl?

    namespace references {
        inline util::memory_reference<zombie_manager*> zombies { 0x0102FFF0 };
    } // references
} // treyarch
