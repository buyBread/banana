#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class slc_arch_base_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_arch_base_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    class slf__wait__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_beam__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait__arch_base__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait_timeout__arch_base__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait_a_frame__arch_base__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__arch_base__clear_callbacks__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__arch_base__clear_callback__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__arch_base__clear_callback__uint__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    namespace references {
        inline util::memory_reference<slc_arch_base_t*> slc_arch_base { 0x010F70BC };
    } // references
} // treyarch
