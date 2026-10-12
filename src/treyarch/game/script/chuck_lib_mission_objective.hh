#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class slc_mission_objective_data_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_mission_objective_data_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    class slc_mission_stage_data_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_mission_stage_data_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    namespace references {
        inline util::memory_reference<slc_mission_objective_data_t*> slc_mission_objective_data { 0x010F70FC };
        inline util::memory_reference<slc_mission_stage_data_t*>     slc_mission_stage_data     { 0x010F7100 };
    } // references
} // treyarch
