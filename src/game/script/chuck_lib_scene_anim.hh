#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class slc_cut_scene_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_cut_scene_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    class slf__create_cut_scene__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_preregistered_cut_scene__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait_play__cut_scene__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__stop_cut_scene__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait_run_toa_cutscene__dynamic_array__dynamic_array__dynamic_array__dynamic_array__dynamic_array__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    namespace references {
        inline util::memory_reference<slc_cut_scene_t*> slc_cut_scene { 0x010F70E4 };
    } // references
} // treyarch
