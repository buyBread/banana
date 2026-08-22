#pragma once

#include "treyarch/amalga/file.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace resources {
    void init();

    namespace references {
        inline util::memory_reference<u32>   default_package_size { 0x00F55004 };
        inline util::memory_reference<u8>    default_package_data { 0x00F56000 };
        inline util::memory_reference<void*> default_package_copy { 0x01118514 };
    } // references
}}} // treyarch::ngl::resources
