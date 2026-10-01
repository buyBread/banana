#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class game_meter_manager; // impl?

    namespace references {
        inline util::memory_reference<game_meter_manager*> game_meter_manager { 0x010F9BD4 };
    } // references
} // treyarch
