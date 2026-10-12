#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class slc_cpu_combat_movement_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_cpu_combat_movement_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}

        virtual void unk_008(u32, u32, u32) {}
    };

    class slc_generic_event_callback_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_generic_event_callback_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}

        virtual void unk_008(u32, u32, u32) {}
    };

    namespace references {
        inline util::memory_reference<slc_cpu_combat_movement_t*>    slc_cpu_combat_movement    { 0x010F70CC };
        inline util::memory_reference<slc_generic_event_callback_t*> slc_generic_event_callback { 0x010F70D0 };
    } // references
} // treyarch
