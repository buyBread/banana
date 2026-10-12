#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class slc_trigger_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_trigger_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}

        u32 find_instance(const mash::string &name) const override;

        virtual void unk_008(u32, u32, u32) {}
    };

    namespace references {
        inline util::memory_reference<slc_trigger_t*> slc_trigger { 0x010F70C4 };
    } // references
} // treyarch
