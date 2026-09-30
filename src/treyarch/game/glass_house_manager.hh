#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class glass_house_manager; // impl?

    namespace references {
        inline util::memory_reference<glass_house_manager*> glass_house_manager { 0x0102FEC4 };
    } // references
} // treyarch
