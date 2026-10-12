#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class slc_arch_base_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_arch_base_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    namespace references {
        inline util::memory_reference<slc_arch_base_t*> slc_arch_base { 0x010F70BC };
    } // references
} // treyarch
