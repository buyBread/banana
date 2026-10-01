#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class quest_manager; // impl?

    namespace references {
        inline util::memory_reference<quest_manager*> quest_manager { 0x01087FD4 };
    } // references
} // treyarch
