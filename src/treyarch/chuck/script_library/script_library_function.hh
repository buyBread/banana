#pragma once

#include "treyarch/chuck/vm/vm_stack.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    struct script_library_function; 
    struct script_library_function_vector;
    struct script_library_class;
    
    namespace fn {
        inline auto script_library_function_construct_for_class =
            (script_library_function*(__thiscall*)
            (script_library_function*, script_library_class*))
            0x00A215E0;

        inline auto script_library_function_construct_global =
            (script_library_function*(__thiscall*)
            (script_library_function*))
            0x00A21650;
    } // fn

    /*
        script_library_function is a stateless native-call descriptor.
        only has one field, which is the implicit vtable pointer and its only virtual slot invokes the native function.
        script_library_class owns these descriptors and releases their engine allocations directly.
    */

    struct script_library_function {
        /*
            returning true:  completes the call and continues
            returning false: restores the bytecode PC & stack offset
                             and will retry the same BSL instruction
        */
        using callback_t = bool(__thiscall*)(
            const script_library_function*,
            vm::vm_stack&);

        // construction order is ABI-sensitive

        script_library_function() { 
            fn::script_library_function_construct_global(this);
        }

        explicit script_library_function(script_library_class* owner) {
            fn::script_library_function_construct_for_class(this, owner);
        }

        virtual bool operator()(vm::vm_stack&) const {
            return true;
        }

        void** vtable() const noexcept {
            return *(void***)this;
        }

        callback_t callback() const noexcept {
            callback_t* callbacks = (callback_t*)vtable();

            return callbacks ? callbacks[0] : nullptr;
        }
    };

    struct script_library_function_vector {
        u32                       unk_00;
        script_library_function** first;
        script_library_function** last;
        script_library_function** capacity_end;

        u32 size() const noexcept {
            if (!first)
                return 0;

            return (u32)(last - first);
        }

        bool empty() const noexcept {
            return size() == 0;
        }

        script_library_function* operator[](u32 index) const noexcept {
            return first[index];
        }
    };
}}} // treyarch::chuck::script_library