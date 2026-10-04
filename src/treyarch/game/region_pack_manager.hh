#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class region_pack_manager {

    public:
        void** vtable;
    };

    namespace references {
        // created through sub_93D700 by region_spawn_manager's constructor (sub_951A90)
        inline util::memory_reference<region_pack_manager*> region_pack_manager { 0x010FB18C };
    } // references
} // treyarch
